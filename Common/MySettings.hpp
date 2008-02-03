/**
 * MySettings.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MYSETTINGS_HPP
#define MYSETTINGS_HPP MYSETTINGS_HPP

#include <QSettings>
class QMainWindow;

class MySettings : public QSettings
{
public:
   enum notificationMode { disable, enable, update };
   
   MySettings();

   /* set a value that's shared between all applications or from another application */
   void setGlobalValue( const QString &key, const QVariant &value,
                        const QString &application = QString() );
   /* get a value that's shared between all applications or from another application */
   QVariant globalValue( const QString &key, const QVariant &defaultValue = QVariant(),
                         const QString &application = QString() );

   /* save main window settings */
   void saveMainWindow( QMainWindow *mainWindow );
   /* restore main window settings */
   void setMainWindow( QMainWindow *mainWindow );

   /* check if the calling application listens on the provided application */
   bool listensOn( const QString &application );
   /* send a message to another application idenfied by port */
   void sendUdpMessage( const QString &data, int port = 0 );
   /* send a message to another application idenfied by application name */
   void sendUdpMessage( const QString &data, const QString &application );
   /* send notification to all it may concern */
   void sendNotification( const QString &data );
};

#endif
