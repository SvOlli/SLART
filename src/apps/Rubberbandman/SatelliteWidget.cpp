/*
 * src/apps/Rubberbandman/SatelliteWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SatelliteWidget.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QPushButton>
#include <QString>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <GenericSatelliteHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>

/* local headers */
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"

#include "Trace.hpp"


SatelliteWidget::SatelliteWidget( QWidget *parent )
: QWidget( parent )
, mpDatabase( DatabaseInterface::get() )
, mpInfoEdit( new InfoEdit( this ) )
, mpSatellite( Satellite::get() )
, mpGenericSatelliteHandler( 0 )
, mpNowPlaying( new QPushButton( tr("NP: To Clipboard"), this ) )
, mpShowInFilesystem( new QPushButton( tr("Show In Filesystem"), this ) )
, mpGetRandom( new QPushButton( tr("Get Random Track"), this ) )
, mpEnqueue( new QPushButton( tr("Enqueue in Partyman"), this ) )
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
   buttonLayout->addWidget( mpEnqueue );
   mainLayout->addStretch();
   mainLayout->addLayout( buttonLayout );
   mainLayout->addStretch();
   mainLayout->addWidget( mpInfoEdit );
   setLayout(mainLayout);

   connect( mpNowPlaying, SIGNAL(clicked()),
            this, SLOT(handleNowPlaying()) );
   connect( mpShowInFilesystem, SIGNAL(clicked()),
            this, SLOT(handleShowInFilesystem()) );
   connect( mpGetRandom, SIGNAL(clicked()),
            this, SLOT(handleGetRandom()) );
   connect( mpEnqueue, SIGNAL(clicked()),
            this, SLOT(handleEnqueue()) );

   if( mpSatellite )
   {
      mpGenericSatelliteHandler = new GenericSatelliteHandler( mpSatellite, GenericSatelliteHandler::WithPingAndDialog, this );
      mpGenericSatelliteHandler->setConnectMsg( "P0R" );
      connect( mpSatellite, SIGNAL(received(QByteArray)),
               this, SLOT(handleSatellite(QByteArray)) );
      mpDatabase->registerUpdate( mpSatellite, "r0u" );
   }
}


SatelliteWidget::~SatelliteWidget()
{
}


void SatelliteWidget::handleSatellite( const QByteArray &msg )
{
   QStringList message( Satellite::split( msg ) );

   if( message.size() > 0 )
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
      if( message.at(0) == "R0T" )
      {
         if( message.size() > 1 )
         {
            message.takeFirst();
            mpDatabase->generateTestLoad( this, SLOT(handleGetRandom()), message.join(" ") );
         }
         else
         {
            mpDatabase->generateTestLoad( this, SLOT(handleGetRandom()), "cr cd dk sv dt vc" );
         }
      }
   }
}


void SatelliteWidget::handleNowPlaying()
{
   GlobalConfigWidget::setClipboard( mpInfoEdit->tagsFileName(
      Settings::value( Settings::RubberbandmanPlayingPattern ) ) );
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
   mpDatabase->getRandomTrack( mpInfoEdit, SLOT(loadTrackInfo(TrackInfo)), false, false, QStringList() );
}


void SatelliteWidget::handleEnqueue()
{
   if( !(mpInfoEdit->fileName().isEmpty()) )
   {
      emit mpSatellite->send( QString("P0Q\n%1").arg(mpInfoEdit->fileName()).toUtf8() );
   }
}
