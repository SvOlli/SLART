/*
 * src/apps/Rubberbandman/DirWalkerCallbackProxy.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DirWalkerCallbackProxy.hpp"

/* system headers */

/* Qt headers */
#include <QMetaObject>
#include <QObject>

/* local library headers */

/* local headers */


DirWalkerCallbackProxy::DirWalkerCallbackProxy( QObject *target, enum Qt::ConnectionType connectionType )
: mpTarget( target )
, mConnectionType( connectionType )
{
}


DirWalkerCallbackProxy::~DirWalkerCallbackProxy()
{
}


void DirWalkerCallbackProxy::handleStart()
{
   QMetaObject::invokeMethod( mpTarget, "handleStart", mConnectionType );
}


void DirWalkerCallbackProxy::handleFile( const QFileInfo &fileInfo )
{
   QMetaObject::invokeMethod( mpTarget, "handleFile", mConnectionType,
                              Q_ARG( const QFileInfo&, fileInfo ) );
}


void DirWalkerCallbackProxy::handleDirEntry( const QFileInfo &fileInfo )
{
   QMetaObject::invokeMethod( mpTarget, "handleDirEntry", mConnectionType,
                              Q_ARG( const QFileInfo&, fileInfo ) );
}


void DirWalkerCallbackProxy::handleDirLeave( const QFileInfo &fileInfo )
{
   QMetaObject::invokeMethod( mpTarget, "handleDirLeave", mConnectionType,
                              Q_ARG( const QFileInfo&, fileInfo ) );
}


void DirWalkerCallbackProxy::handleOther( const QFileInfo &fileInfo )
{
   QMetaObject::invokeMethod( mpTarget, "handleOther", mConnectionType,
                              Q_ARG( const QFileInfo&, fileInfo ) );
}


void DirWalkerCallbackProxy::handleEnd()
{
   QMetaObject::invokeMethod( mpTarget, "handleEnd", mConnectionType );
}
