/**
 * Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

//#include <unistd.h>

#include <lirc/lirc_client.h>
#include <QString>
#include <QSettings>
#include <QUdpSocket>

#include "Trace.hpp"

#define PROGNAME "Creep"


int main(int argc, char *argv[])
{
   struct lirc_config *config;

   if(argc>2)
   {
      fprintf(stderr,"Usage: %s <config file>\n",argv[0]);
      exit(EXIT_FAILURE);
   }
   if(lirc_init(PROGNAME,1)==-1) exit(EXIT_FAILURE);

   if(lirc_readconfig(argc==2 ? argv[1]:NULL,&config,NULL)==0)
   {
      char *code;
      char *c;
      int ret;

      while(lirc_nextcode(&code)==0)
      {
         if(code==NULL) continue;
         while((ret=lirc_code2char(config,code,&c))==0 && c!=NULL)
         {
            QString confline(c);
            int pos = confline.indexOf(' ');
            
            QString application( confline.left( pos ) );
            QString data( confline.mid( pos + 1 ) );
            
            if( application.startsWith( "System", Qt::CaseInsensitive ) )
            {
               system( data.toUtf8().data() );
               continue;
            }
            
            /* copy'n'paste from MySettings.cpp */
            int port = QSettings( "SLART", application )
                                 .value( "UDPListenerPort", 0 ).toInt();
            
            if( (port < 1) || (port > 65535) )
            {
               continue;
            }
            
            QUdpSocket().writeDatagram( data.toUtf8(), QHostAddress::LocalHost, port );
         }
         free(code);
         if(ret==-1) break;
      }
      lirc_freeconfig(config);
   }

   lirc_deinit();
   exit(EXIT_SUCCESS);
}
