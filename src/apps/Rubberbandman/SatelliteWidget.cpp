/**
 * src/apps/Rubberbandman/SatelliteWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SatelliteWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QString>

/* local library headers */
#include <Database.hpp>
#include <GenericSatMsgHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"

#include "Trace.hpp"


SatelliteWidget::SatelliteWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpInfoEdit( new InfoEdit( database ) )
, mpSatellite( Satellite::get( this ) )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite, GenericSatMsgHandler::WithPingAndDialog ) )
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


void SatelliteWidget::handleSatellite( const QByteArray &msg )
{
   QStringList message( Satellite::split( msg ) );

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


void SatelliteWidget::handleNowPlaying()
{
   GlobalConfigWidget::setClipboard( mpInfoEdit->tagsFileName( 
      MySettings().VALUE_PLAYINGPATTERN ) );
}


void SatelliteWidget::handleShowInFilesystem()
{
   if( !(mpInfoEdit->fileName().isEmpty()) )
   {
      emit showInFilesystem( mpInfoEdit->fileName() );
   }
}


void SatelliteWidget::handleGetRandom()
{
TRACESTART(SatelliteWidget::handleGetRandom)
   TrackInfo trackInfo;
   if( mpDatabase->getRandomTrack( &trackInfo, false, true ) )
   {
      mpInfoEdit->load( trackInfo.mDirectory + "/" + trackInfo.mFileName );
TRACEMSG << trackInfo.toString();
   }
}
