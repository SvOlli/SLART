/**
 * SLATCom.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLATCom_HPP
#define SLATCom_HPP SLATCom_HPP

#include <QUdpSocket>
class MySettings;
class QStringList;


class SLATCom : public QUdpSocket
{
Q_OBJECT

public:
   SLATCom( QObject *parent = 0 );

   void startReceiver();
   void stopReceiver();
   void sendNotification( const QString &data );

public slots:
   void handleReadyRead();
   
signals:
   void updateConfig();
   void packageRead( const QStringList &data );
   
private:
   void updateListeners( MySettings *settings, const QStringList &listeners );

   QObject     *mpParent;
   QList<int>  mPorts;
};

#endif
