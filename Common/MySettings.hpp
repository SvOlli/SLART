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

   void setGlobalValue( const QString &key, const QVariant &value,
                        const QString &application = QString() );
   QVariant globalValue( const QString &key, const QVariant &defaultValue = QVariant(),
                         const QString &application = QString() );

   void saveMainWindow( QMainWindow *mainWindow );
   void setMainWindow( QMainWindow *mainWindow );

   void sendUdpMessage( const QByteArray &data, int port = 0 );
   void sendUdpMessage( const QByteArray &data, const QString &application );
};

#endif
