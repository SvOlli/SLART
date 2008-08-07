/**
 * SLARTComWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTComWidget.hpp"

#include <QtGui>
#include <QString>

#include "Database.hpp"
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


SLARTComWidget::SLARTComWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpInfoEdit( new InfoEdit( database ) )
, mpNowPlaying( new QPushButton( tr("NP: To Clipboard"), this ) )
, mpShowInFilesystem( new QPushButton( tr("Show In Filesystem"), this ) )
, mpGetRandom( new QPushButton( tr("Get Random Track"), this ) )
, mSLARTCom()
{
   QBoxLayout *mainLayout;
   QBoxLayout *buttonLayout;
   
   if( QApplication::desktop()->screenGeometry().height() < 600 )
   {
      mainLayout   = new QHBoxLayout( this );
      buttonLayout = new QVBoxLayout;
   }
   else
   {
      mainLayout   = new QVBoxLayout( this );
      buttonLayout = new QHBoxLayout;
   }
   
   buttonLayout->addWidget( mpNowPlaying );
   buttonLayout->addWidget( mpShowInFilesystem );
   buttonLayout->addWidget( mpGetRandom );
   mainLayout->addStretch();
   mainLayout->addLayout( buttonLayout );
   mainLayout->addStretch();
   mainLayout->addWidget( mpInfoEdit );
   setLayout(mainLayout);
   
   connect( &mSLARTCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLART(QStringList)) );
   connect( &mSLARTCom, SIGNAL(updateConfig()),
            this, SLOT(readConfig()) );
   connect( mpNowPlaying, SIGNAL(clicked()),
            this, SLOT(handleNowPlaying()) );
   connect( mpShowInFilesystem, SIGNAL(clicked()),
            this, SLOT(handleShowInFilesystem()) );
   connect( mpGetRandom, SIGNAL(clicked()),
            this, SLOT(handleGetRandom()) );
   
   readConfig();
}


void SLARTComWidget::handleSLART( const QStringList &message )
{
   if( (message.at(0) == "p0p") && (message.size() > 1) )
   {
      mpInfoEdit->load( message.at(1) );
   }
   if( (message.at(0) == "p0u") || (message.at(0) == "k0u") )
   {
      mpInfoEdit->load( QString("-") );
   }
   if( message.at(0) == "p0c" )
   {
      emit partymanConfigUpdate();
   }
}


void SLARTComWidget::handleNowPlaying()
{
   GlobalConfigWidget::setClipboard( mpInfoEdit->tagsFileName( 
      MySettings().value("PlayingPattern", "NP: |$ARTIST| - |$TITLE|").toString(), false ) );
}


void SLARTComWidget::handleShowInFilesystem()
{
   if( !(mpInfoEdit->fileName().isEmpty()) )
   {
      emit showInFilesystem( mpInfoEdit->fileName() );
   }
}


void SLARTComWidget::handleGetRandom()
{
TRACESTART(SLARTComWidget::handleGetRandom)
   TrackInfo trackInfo;
   if( mpDatabase->getRandomTrack( &trackInfo, false, true ) )
   {
      mpInfoEdit->load( trackInfo.mDirectory + "/" + trackInfo.mFileName );
TRACEMSG << trackInfo.toString();
   }
}


void SLARTComWidget::readConfig()
{
   mSLARTCom.resetReceiver();
}
