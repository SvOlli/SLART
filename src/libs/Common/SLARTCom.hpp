/**
 * src/libs/Common/SLARTCom.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SLARTCOM_HPP
#define SLARTCOM_HPP SLARTCOM_HPP

/*
 * this class is only for receiving SLARTCom messages 
 * sending of SLARTCom messages is handled by the MySettings class
 */

#include <QUdpSocket>

class QStringList;

class MySettings;


class SLARTCom : public QObject
{
Q_OBJECT
   
public:
   SLARTCom( QObject *parent = 0 );
   
   /* bind or release the port according to settings */
   void resetReceiver();
   /* send a ping request to the specified application */
   bool ping( const QString &application );
   
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
   
   QObject      *mpParent;
   QUdpSocket   mUdpSocket;
   bool         mEndPing;
   int          mPingPort;
};

#endif
