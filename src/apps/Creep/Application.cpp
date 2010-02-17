/**
 * src/apps/Creep/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

//#include <unistd.h>

#include <lirc/lirc_client.h>
#include <QString>
#include <QSettings>
#include <QHostAddress>
#include <QTcpSocket>

#include "../../libs/Common/Satellite.hpp"


static char GROUPNAME[] = "SLART";
static char PROGNAME[] = "Creep";


static void send( const QString &data )
{
   /* copy'n'paste from Satellite.cpp */
   QTcpSocket socket;
   QSettings  settings( GROUPNAME, "Global" );

   QHostAddress host( settings.VALUE_SATELLITE_HOST );
   qint16       port( settings.VALUE_SATELLITE_PORT );

   socket.connectToHost( host, port, QIODevice::WriteOnly );
   if( socket.waitForConnected( 1000 ) )
   {
      SATELLITE_HEADER_TYPE msgSize = data.size();
      socket.write( (char*)(&msgSize), SATELLITE_HEADER_SIZE );
      socket.write( data.toUtf8() );
      socket.flush();
      socket.disconnectFromHost();
   }
}


int main(int argc, char *argv[])
{
   struct lirc_config *config;

   if( argc > 2 )
   {
      fprintf( stderr, "Usage: %s <config file>\n", argv[0] );
      exit( EXIT_FAILURE );
   }
   
   if( lirc_init( &PROGNAME[0] , 1 ) == -1)
   {
      exit( EXIT_FAILURE );
   }

   if( !QSettings( GROUPNAME, PROGNAME ).VALUE_USE_SATELLITE )
   {
      fprintf( stderr, "You need to enable \"Satellite\" (SLART's inter-process communication) for %s\n", PROGNAME );
      exit( EXIT_FAILURE );
   }

   if( lirc_readconfig( argc > 1 ? argv[1] : NULL, &config, NULL ) == 0 )
   {
      char *code;
      char *c;
      int ret;

      while( lirc_nextcode( &code ) == 0 )
      {
         if( code == NULL )
         {
            continue;
         }
         
         while( (( ret = lirc_code2char( config, code, &c ) ) == 0) && (c != NULL) )
         {
            QString confline( c );
            
            if( confline.startsWith( "SYS", Qt::CaseInsensitive ) )
            {
               confline = confline.mid(confline.indexOf(' ')+1);
               if( system( confline.toUtf8().constData() ) )
               {
                  confline.prepend( QString("System call failed:\n") );
                  send( confline );
               }
            }
            else
            {
               send( confline );
            }
            
         }
         
         free( code );
         
         if( ret == -1 ) 
         {
            break;
         }
      }
      lirc_freeconfig( config );
   }

   lirc_deinit();
   exit( EXIT_SUCCESS );
}
