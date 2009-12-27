/**
 * src/libs/SLARTCom/SLARTComServerRunner.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTComServerRunner.hpp"

#include "SLARTComServer.hpp"


SLARTComServerRunner::SLARTComServerRunner( quint16 port, const QHostAddress &host )
: QThread()
, mPort( port )
, mHost( host )
{
}


void SLARTComServerRunner::run()
{
   SLARTComServer *mpServer = new SLARTComServer( mPort, mHost, 0 );
#if SLARTCOMSERVER_DEBUG
   connect( mpServer, SIGNAL(debug(const QString&)),
            this,     SIGNAL(debug(const QString&)) );
#endif
   if( mpServer->listen() )
   {
      exec();
   }
   mpServer->deleteLater();
   this->deleteLater();
}
