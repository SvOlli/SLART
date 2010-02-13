/**
 * SLARTComWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTComWidget.hpp"

#include <QtGui>
#include <QString>

#include "ConfigDialog.hpp"
#include "Database.hpp"
#include "FileSysBrowser.hpp"
#include "GlobalConfigWidget.hpp"
#include "InfoEdit.hpp"
#include "MySettings.hpp"
#include "Satellite.hpp"

#include "Trace.hpp"


SLARTComWidget::SLARTComWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpInfoEdit( new InfoEdit( database ) )
, mpSatellite( Satellite::get( this ) )
, mpNowPlaying( new QPushButton( tr("NP: To Clipboard"), this ) )
, mpShowInFilesystem( new QPushButton( tr("Show In Filesystem"), this ) )
, mpGetRandom( new QPushButton( tr("Get Random Track"), this ) )
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
   
   connect( mpSatellite, SIGNAL(received(const QByteArray &)),
            this, SLOT(handleSatellite(const QByteArray &)) );
   connect( mpNowPlaying, SIGNAL(clicked()),
            this, SLOT(handleNowPlaying()) );
   connect( mpShowInFilesystem, SIGNAL(clicked()),
            this, SLOT(handleShowInFilesystem()) );
   connect( mpGetRandom, SIGNAL(clicked()),
            this, SLOT(handleGetRandom()) );
   
   mpSatellite->restart();
}


void SLARTComWidget::handleSatellite( const QByteArray &msg )
{
   QStringList message( QString::fromUtf8( msg ).split('\n') );

   if( message.size() > 0 )
   {
      if( (message.at(0) == "CFG") )
      {
         mpSatellite->restart();
      }
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
}


void SLARTComWidget::handleNowPlaying()
{
   GlobalConfigWidget::setClipboard( mpInfoEdit->tagsFileName( 
      MySettings().VALUE_PLAYINGPATTERN ) );
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
