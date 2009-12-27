/**
 * src/libs/SLARTCom/SLARTComServer.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTCOMSERVERRUNNER_HPP
#define SLARTCOMSERVERRUNNER_HPP SLARTCOMSERVERRUNNER_HPP

#include <QThread>

#include <QHostAddress>

//#include "SLARTComServer.hpp"


class SLARTComServer;

class SLARTComServerRunner : public QThread
{
Q_OBJECT

public:
   SLARTComServerRunner( quint16 port, const QHostAddress &host );
   void run();

signals:
#if SLARTCOMSERVER_DEBUG
   /* output debug messages (depricated) */
   void debug( const QString &message );
#endif

private:
   SLARTComServer      *mpServer;
   const quint16        mPort;
   const QHostAddress   mHost;
};
#endif
