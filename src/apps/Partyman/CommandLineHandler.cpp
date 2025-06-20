/*
 * src/apps/Partyman/CommandLineHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */


/* class declaration */
#include "CommandLineHandler.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QStringList>

/* local library headers */
#include <GenericSatelliteHandler.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>

/* local headers */


CommandLineHandler::CommandLineHandler( const QStringList &list, QObject *parent )
: QObject( parent )
, mpSatellite( Satellite::get() )
, mpGenericSatelliteHandler( 0 )
, mConnected( false )
, mList()
{
   QFileInfo fileInfo;
   foreach( const QString &file, list )
   {
      fileInfo.setFile( file );
      mList << fileInfo.absoluteFilePath();
   }
   if( mpSatellite )
   {
      mpGenericSatelliteHandler = new GenericSatelliteHandler( mpSatellite, GenericSatelliteHandler::WithPing, this );
      connect( mpGenericSatelliteHandler, SIGNAL(anotherInstance()),
               this, SLOT(gotPing()) );
   }
   connect( this, SIGNAL(done()),
            qApp, SLOT(quit()) );
   QTimer::singleShot( 1000, this, SLOT(timeout()) );
}


CommandLineHandler::~CommandLineHandler()
{
}


void CommandLineHandler::gotPing()
{
   mConnected = true;
   QString fileName;
   foreach( fileName, mList )
   {
      mpSatellite->send( fileName.prepend( "P0Q\n" ).toUtf8() );
   }
   emit done();
   deleteLater();
}


void CommandLineHandler::timeout()
{
   if( !mConnected )
   {
      QStringList list( Settings::value( Settings::PartymanPlaylistAppend ) );
      list << mList;
      Settings::setValue( Settings::PartymanPlaylistAppend, list );
      emit done();
      deleteLater();
   }
}
