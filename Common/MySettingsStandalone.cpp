/**
 * MySettingsStandalone.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

MySettings::MySettings()
: QSettings()
{
}


void MySettings::setGlobalValue( const QString &key, const QVariant &value,
                                 const QString &/*application*/ )
{
   setValue( key, value );
}


QVariant MySettings::globalValue( const QString &key, const QVariant &defaultValue,
                                  const QString &/*application*/ )
{
   return value( key, defaultValue );
}


void MySettings::sendUdpMessage( const QString &/*data*/, int /*port*/ )
{
   /* no sending messages in standalone applications */
}


void MySettings::sendUdpMessage( const QString &/*data*/, const QString &/*application*/ )
{
   /* no sending messages in standalone applications */
}


void MySettings::requestNotification( const QString &/*application*/, bool /*enable*/ )
{
   /* no sending messages in standalone applications */
}


bool MySettings::listensOn( const QString &/*application*/ )
{
   /* no sending messages in standalone applications */
}
