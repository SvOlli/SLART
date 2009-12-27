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
#include <QUdpSocket>

static char PROGNAME[] = "Creep";


static void send( const QString &application, const QString &data )
{
   /* copy'n'paste from MySettings.cpp */
   int port = QSettings( "SLART", application )
                        .value( "UDPListenerPort", 0 ).toInt();
   
   if( (port < 1) || (port > 65535) )
   {
      return;
   }
   
   QUdpSocket().writeDatagram( data.toUtf8(), QHostAddress::LocalHost, port );
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
            int pos = confline.indexOf( ' ' );
            
            QString application( confline.left( pos ) );
            QString data( confline.mid( pos + 1 ) );
            
            if( application.startsWith( "System", Qt::CaseInsensitive ) )
            {
               if( system( data.toUtf8().data() ) )
               {
                  data.prepend( QString("System call failed:\n") );
                  send( QString("Innuendo"), data );
               }
            }
            else
            {
               send( application, data );
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
