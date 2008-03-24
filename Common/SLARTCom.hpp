/**
 * SLARTCom.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTCOM_HPP
#define SLARTCOM_HPP SLARTCOM_HPP

#include <QUdpSocket>
class MySettings;
class QStringList;


/* sending of SLARTCom messages is handle by the MySettings class */

class SLARTCom : public QObject
{
Q_OBJECT

public:
   SLARTCom( QObject *parent = 0 );

   /* bind or release the port according to settings */
   void resetReceiver();

public slots:
   /* handle new incoming udp packet */
   void handleReadyRead();
   
signals:
   /* got SLART message to reread config */
   void updateConfig();
   /* send the processed SLART message */
   void packageRead( const QStringList &data );
   
private:
   SLARTCom( const SLARTCom &other );
   SLARTCom &operator=( const SLARTCom &other );

   QObject     *mpParent;
   QUdpSocket  mUdpSocket;
};

#endif
