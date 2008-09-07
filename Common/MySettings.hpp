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
   MySettings();
   MySettings( const QString &application );
   
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
