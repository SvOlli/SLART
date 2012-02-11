/*
 * src/libs/Common/MagicEncoderProxy.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderProxy.hpp"

/* system headers */

/* Qt headers */
#include <QPluginLoader>

/* local library headers */
#include "../../libs/Magic/MagicEncoderInterface.hpp"
#include <Satellite.hpp>

/* local headers */


MagicEncoderProxy::MagicEncoderProxy( Satellite *satellite, QObject *parent )
: QObject( parent )
, mpPluginLoader( new QPluginLoader( this ) )
, mpPlugin( 0 )
, mpSatellite( satellite )
{
}


MagicEncoderProxy::~MagicEncoderProxy()
{
}


bool MagicEncoderProxy::pluginLoad( const QString &fileName, const QString &msgHeader )
{
   mpPluginLoader->setFileName( fileName );
   QObject *plugin = mpPluginLoader->instance();
   mpPlugin = qobject_cast<MagicEncoderInterface*>(plugin);
   if( mpPlugin )
   {
      mpPlugin->setup( this, msgHeader );
      return true;
   }
   return false;
}


QString MagicEncoderProxy::pluginLoadErrorString()
{
   return mpPluginLoader->errorString();
}



QString MagicEncoderProxy::pluginFileName()
{
   return mpPluginLoader->fileName();
}


MagicEncoderConfig *MagicEncoderProxy::configWidget( QWidget *parent, QAbstractButton *button )
{
   if( !mpPlugin )
   {
      return 0;
   }
   return mpPlugin->configWidget( parent, button );
}


QThread *MagicEncoderProxy::workerThread()
{
   if( !mpPlugin )
   {
      return 0;
   }
   return mpPlugin->workerThread();
}


QString MagicEncoderProxy::name()
{
   if( !mpPlugin )
   {
      return QString();
   }
   return mpPlugin->name();
}


void MagicEncoderProxy::emitEncodingFail()
{
   emit encodingFail();
}


void MagicEncoderProxy::setTags( const TagList &tagList )
{
   if( !mpPlugin )
   {
      return;
   }
   mpPlugin->setTags( tagList );
}


bool MagicEncoderProxy::useEncoder()
{
   if( !mpPlugin )
   {
      return false;
   }
   return mpPlugin->useEncoder();
}


void MagicEncoderProxy::setEnqueue( bool activate )
{
   if( !mpPlugin )
   {
      return;
   }
   mpPlugin->setEnqueue( activate );
}


bool MagicEncoderProxy::initialize( const QString &fileName )
{
   if( !mpPlugin )
   {
      return false;
   }
   return mpPlugin->initialize( fileName );
}


bool MagicEncoderProxy::finalize( bool enqueue, bool cancel )
{
   if( !mpPlugin )
   {
      return false;
   }
   return mpPlugin->finalize( enqueue, cancel );
}


void MagicEncoderProxy::satelliteSend( const QByteArray &data )
{
   if( mpSatellite )
   {
      mpSatellite->send( data );
   }
   else
   {
      Satellite::send1( data );
   }
}


void MagicEncoderProxy::encodeCDAudio( const QByteArray &data )
{
   if( !mpPlugin )
   {
      return;
   }
   mpPlugin->encodeCDAudio( data );
}
