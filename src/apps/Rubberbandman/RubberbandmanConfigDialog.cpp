/*
 * src/apps/Rubberbandman/RubberbandmanConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "RubberbandmanConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */


RubberbandmanConfigDialog::RubberbandmanConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpAutoRename( new QCheckBox( tr("Rename Files To Match Pattern"), this ) )
, mpAutoRescan( new QCheckBox( tr("Rescan After Filesystem Operation"), this ) )
, mpWithTrackNrLabel( new QLabel( tr("Pattern For Rename\nWith Track Nr"), this ) )
, mpWithTrackNr( new QLineEdit( this ) )
, mpWithTrackNrExample( new QLabel( this ) )
, mpWithoutTrackNrLabel( new QLabel( tr("Pattern For Rename\nWithout Track Nr"), this ) )
, mpWithoutTrackNr( new QLineEdit( this ) )
, mpWithoutTrackNrExample( new QLabel( this ) )
, mpPlayingPatternLabel( new QLabel( tr("Pattern For\nNow Playing"), this ) )
, mpPlayingPattern( new QLineEdit( this ) )
, mpPlayingPatternExample( new QLabel( this ) )
, mTagMap()
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon( ":/Rubberbandman/Icon.png" ) );

   mpWithTrackNrLabel->setAlignment( Qt::AlignTop );
   mpWithoutTrackNrLabel->setAlignment( Qt::AlignTop );
   mpPlayingPatternLabel->setAlignment( Qt::AlignTop );

   mTagMap.insert("TRACKNUMBER","1");
   mTagMap.insert("ALBUMARTIST","AlbumArtist");
   mTagMap.insert("ALBUM","Album");
   mTagMap.insert("ARTIST","Artist");
   mTagMap.insert("TITLE","Title");
   mTagMap.insert("GENRE","Genre");
   mTagMap.insert("DATE","1986");
   mTagMap.insert("DIRECTORY","/path/to");
   mTagMap.insert("FILENAME","filename.ext");

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();
   mpGlobalConfigWidget->showAnimate();
   mpGlobalConfigWidget->showNormalize();
   mpGlobalConfigWidget->showDoubleClickInterval();

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QWidget     *rbmTab    = new QWidget( this );
   QGridLayout *rbmLayout = new QGridLayout( rbmTab );
   rbmLayout->addWidget( mpAutoRename,             0, 0, 1, 2 );
   rbmLayout->addWidget( mpAutoRescan,             1, 0, 1, 2 );
   rbmLayout->addWidget( mpWithTrackNrLabel,       2, 0, 2, 1 );
   rbmLayout->addWidget( mpWithTrackNr,            2, 1 );
   rbmLayout->addWidget( mpWithTrackNrExample,     3, 1 );
   rbmLayout->addWidget( mpWithoutTrackNrLabel,    4, 0, 2, 1 );
   rbmLayout->addWidget( mpWithoutTrackNr,         4, 1 );
   rbmLayout->addWidget( mpWithoutTrackNrExample,  5, 1 );
   rbmLayout->addWidget( mpPlayingPatternLabel,    6, 0, 2, 1 );
   rbmLayout->addWidget( mpPlayingPattern,         6, 1 );
   rbmLayout->addWidget( mpPlayingPatternExample,  7, 1 );
   rbmLayout->setRowStretch( 8, 1 );
   rbmTab->setLayout( rbmLayout );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( rbmTab,               tr("Rubberbandman") );
   tabs->addTab( mpGlobalConfigWidget, tr("Global") );

   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );

   setLayout( mainLayout );

   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   connect( mpWithTrackNr, SIGNAL(textChanged(QString)),
            this, SLOT(updateWithTrackNr(QString)) );
   connect( mpWithoutTrackNr, SIGNAL(textChanged(QString)),
            this, SLOT(updateWithoutTrackNr(QString)) );
   connect( mpPlayingPattern, SIGNAL(textChanged(QString)),
            this, SLOT(updatePlayingPattern(QString)) );
   connect( mpAutoRename, SIGNAL(toggled(bool)),
            mpAutoRescan, SLOT(setEnabled(bool)) );
   connect( mpAutoRename, SIGNAL(toggled(bool)),
            mpWithoutTrackNr, SLOT(setEnabled(bool)) );
   connect( mpAutoRename, SIGNAL(toggled(bool)),
            mpWithTrackNr, SLOT(setEnabled(bool)) );

   // workaround to make sure that slots are called
   mpAutoRename->setChecked( true );
   readSettings();

   WidgetShot::addWidget( "Config", this );
}


RubberbandmanConfigDialog::~RubberbandmanConfigDialog()
{
}


int RubberbandmanConfigDialog::exec()
{
   readSettings();
   return QDialog::exec();
}


void RubberbandmanConfigDialog::readSettings()
{
   mpAutoRename->setChecked( Settings::value( Settings::RubberbandmanAutoRename ) );
   mpAutoRescan->setChecked( Settings::value( Settings::RubberbandmanAutoRescan ) );
   mpWithTrackNr->setText( Settings::value( Settings::RubberbandmanWithTrackNr ) );
   mpWithoutTrackNr->setText( Settings::value( Settings::RubberbandmanWithoutTrackNr ) );
   mpPlayingPattern->setText( Settings::value( Settings::RubberbandmanPlayingPattern ) );

   mpGlobalConfigWidget->readSettings();

   emit configChanged();
}


void RubberbandmanConfigDialog::writeSettings()
{
   Settings::setValue( Settings::RubberbandmanAutoRename, mpAutoRename->isChecked() );
   Settings::setValue( Settings::RubberbandmanAutoRescan, mpAutoRescan->isChecked() );
   Settings::setValue( Settings::RubberbandmanWithTrackNr, mpWithTrackNr->text() );
   Settings::setValue( Settings::RubberbandmanWithoutTrackNr, mpWithoutTrackNr->text() );
   Settings::setValue( Settings::RubberbandmanPlayingPattern, mpPlayingPattern->text() );

   mpGlobalConfigWidget->writeSettings();

   emit configChanged();
}


void RubberbandmanConfigDialog::updateWithTrackNr( const QString &text )
{
   mpWithTrackNrExample->setText( mTagMap.fileName(text+".ext", true) );
}


void RubberbandmanConfigDialog::updateWithoutTrackNr( const QString &text )
{
   mpWithoutTrackNrExample->setText( mTagMap.fileName(text+".ext", true) );
}


void RubberbandmanConfigDialog::updatePlayingPattern( const QString &text )
{
   mpPlayingPatternExample->setText( mTagMap.fileName(text, false) );
}

