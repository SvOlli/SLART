/**
 * src/apps/Rubberbandman/ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <MySettings.hpp>
#include <WidgetShot.hpp>

/* local headers */


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
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
, mTagList()
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   setWindowIcon( QIcon(":/SLART.png") );
   
   mpWithTrackNrLabel->setAlignment( Qt::AlignTop );
   mpWithoutTrackNrLabel->setAlignment( Qt::AlignTop );
   mpPlayingPatternLabel->setAlignment( Qt::AlignTop );
   
   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","AlbumArtist");
   mTagList.set("ALBUM","Album");
   mTagList.set("ARTIST","Artist");
   mTagList.set("TITLE","Title");
   mTagList.set("GENRE","Genre");
   mTagList.set("DATE","1986");
   
   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();
   mpGlobalConfigWidget->showAnimate();
   mpGlobalConfigWidget->showNormalize();
   mpGlobalConfigWidget->showDoubleClickInterval();
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QWidget     *rbmTab    = new QWidget( this );
   QGridLayout *rbmLayout = new QGridLayout( rbmTab );
   rbmLayout->addWidget( mpAutoRescan, 0, 0, 1, 2 );
   rbmLayout->addWidget( mpWithTrackNrLabel, 1, 0, 2, 1 );
   rbmLayout->addWidget( mpWithTrackNr, 1, 1 );
   rbmLayout->addWidget( mpWithTrackNrExample, 2, 1 );
   rbmLayout->addWidget( mpWithoutTrackNrLabel, 3, 0, 2, 1 );
   rbmLayout->addWidget( mpWithoutTrackNr, 3, 1 );
   rbmLayout->addWidget( mpWithoutTrackNrExample, 4, 1 );
   rbmLayout->addWidget( mpPlayingPatternLabel, 5, 0, 2, 1 );
   rbmLayout->addWidget( mpPlayingPattern, 5, 1 );
   rbmLayout->addWidget( mpPlayingPatternExample, 6, 1 );
   rbmLayout->setRowStretch( 6, 1 );
   rbmTab->setLayout( rbmLayout );
   
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( rbmTab,               QString(tr("Rubberbandman")) );
   tabs->addTab( mpGlobalConfigWidget, QString(tr("Global")) );
   
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
   connect( mpWithTrackNr, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateWithTrackNr(const QString &)) );
   connect( mpWithoutTrackNr, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateWithoutTrackNr(const QString &)) );
   connect( mpPlayingPattern, SIGNAL(textChanged(const QString&)),
            this, SLOT(updatePlayingPattern(const QString &)) );
   
   readSettings();
   
   WidgetShot::addWidget( "ConfigDialog", this );
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   mpAutoRescan->setChecked( settings.VALUE_AUTORESCAN );
   mpWithTrackNr->setText( settings.VALUE_WITHTRACKNR );
   mpWithoutTrackNr->setText( settings.VALUE_WITHOUTTRACKNR );
   mpPlayingPattern->setText( settings.VALUE_PLAYINGPATTERN );
   
   mpGlobalConfigWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   settings.setValue( "AutoRescan", mpAutoRescan->isChecked() );
   settings.setValue( "WithTrackNr", mpWithTrackNr->text() );
   settings.setValue( "WithoutTrackNr", mpWithoutTrackNr->text() );
   settings.setValue( "PlayingPattern", mpPlayingPattern->text() );

   mpGlobalConfigWidget->writeSettings();

   emit configChanged();
}


void ConfigDialog::updateWithTrackNr( const QString &text )
{
   mpWithTrackNrExample->setText( mTagList.fileName(text+".ext", true) );
}


void ConfigDialog::updateWithoutTrackNr( const QString &text )
{
   mpWithoutTrackNrExample->setText( mTagList.fileName(text+".ext", true) );
}


void ConfigDialog::updatePlayingPattern( const QString &text )
{
   mpPlayingPatternExample->setText( mTagList.fileName(text, false) );
}

