/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "GlobalConfigWidget.hpp"
#include "AboutWidget.hpp"


#include <QtGui>
#include "SLARTCom.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpWithTrackNrLabel( new QLabel( tr("Rename Pattern With Track Nr"), this ) )
, mpWithTrackNr( new QLineEdit( this ) )
, mpWithTrackNrExample( new QLabel( this ) )
, mpWithoutTrackNrLabel( new QLabel( tr("Rename Pattern Without Track Nr"), this ) )
, mpWithoutTrackNr( new QLineEdit( this ) )
, mpWithoutTrackNrExample( new QLabel( this ) )
, mpPlayingPatternLabel( new QLabel( tr("Now Playing Pattern"), this ) )
, mpPlayingPattern( new QLineEdit( this ) )
, mpPlayingPatternExample( new QLabel( this ) )
, mTagList()
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","AlbumArtist");
   mTagList.set("ALBUMTITLE","AlbumTitle");
   mTagList.set("ARTIST","Artist");
   mTagList.set("TITLE","Title");
   mTagList.set("GENRE","Genre");
   mTagList.set("Year","1986");
   
   
   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();
   mpGlobalConfigWidget->showAnimate();
   mpGlobalConfigWidget->showNormalize();
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QGroupBox   *rbmGroup  = new QGroupBox( tr("Rubberbandman Settings:"), this );
   QGridLayout *rbmLayout = new QGridLayout( rbmGroup );
   rbmLayout->addWidget( mpWithTrackNrLabel, 0, 0 );
   rbmLayout->addWidget( mpWithTrackNr, 0, 1 );
   rbmLayout->addWidget( mpWithTrackNrExample, 1, 1 );
   rbmLayout->addWidget( mpWithoutTrackNrLabel, 2, 0 );
   rbmLayout->addWidget( mpWithoutTrackNr, 2, 1 );
   rbmLayout->addWidget( mpWithoutTrackNrExample, 3, 1 );
   rbmLayout->addWidget( mpPlayingPatternLabel, 4, 0 );
   rbmLayout->addWidget( mpPlayingPattern, 4, 1 );
   rbmLayout->addWidget( mpPlayingPatternExample, 5, 1 );
   rbmGroup->setLayout( rbmLayout );
   
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   
   mainLayout->addWidget( rbmGroup );
   mainLayout->addWidget( mpGlobalConfigWidget );
   mainLayout->addWidget( about );
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
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   mpWithTrackNr->setText( settings.value("WithTrackNr", "(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|").toString() );
   mpWithoutTrackNr->setText( settings.value("WithoutTrackNr", "|$ARTIST| - |$TITLE|").toString() );
   mpPlayingPattern->setText( settings.value("PlayingPattern", "NP: |$ARTIST| - |$TITLE|").toString() );
   
   mpGlobalConfigWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
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

