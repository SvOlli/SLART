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


/* sending of SLATCom messages is handle by the MySettings class */

class SLATCom : public QObject
{
Q_OBJECT

public:
   SLATCom( QObject *parent = 0 );

   /* bind or release the port according to settings */
   void resetReceiver();
   /* send a message to all notified applications */
   void sendNotification( const QString &data );

public slots:
   /* handle new incoming udp packet */
   void handleReadyRead();
   
signals:
   /* got SLAT message to reread config */
   void updateConfig();
   /* send the processed SLAT message */
   void packageRead( const QStringList &data );
   
private:
   void updateListeners( MySettings *settings, const QStringList &listeners );

   QObject     *mpParent;
   QUdpSocket  mUdpSocket;
   QList<int>  mPorts;
};

#endif
