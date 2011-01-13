/**
 * src/apps/Partyman/CommandLineHandler.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */


/* class declaration */
#include "CommandLineHandler.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QStringList>

/* local library headers */
#include <GenericSatMsgHandler.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "ConfigDialog.hpp"


CommandLineHandler::CommandLineHandler( const QStringList &list, QObject *parent )
: QObject( parent )
, mpSatellite( Satellite::get() )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite, GenericSatMsgHandler::WithPing ) )
, mConnected( false )
, mList( list )
{
   connect( mpGenericSatMsgHandler, SIGNAL(anotherInstance()),
            this, SLOT(gotPing()) );
   connect( this, SIGNAL(done()),
            qApp, SLOT(quit()) );
   QTimer::singleShot( 1000, this, SLOT(timeout()) );
   mpSatellite->restart();
}


CommandLineHandler::~CommandLineHandler()
{
}


void CommandLineHandler::gotPing()
{
   mConnected = true;
   QFileInfo fileInfo;
   foreach( const QString &file, mList )
   {
      fileInfo.setFile( file );
      mpSatellite->send( fileInfo.absoluteFilePath().prepend( "P0Q\n" ).toUtf8() );
   }
   emit done();
}


void CommandLineHandler::timeout()
{
   if( !mConnected )
   {
      MySettings settings;
      QStringList list( settings.value( "PlaylistAppend", QStringList() ).toStringList() );
      list << mList;
      settings.setValue( "PlaylistAppend", list );
      emit done();
   }
}
