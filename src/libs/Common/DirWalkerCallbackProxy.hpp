/**
 * src/apps/Rubberbandman/DirWalkerCallbackProxy.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERCALLBACKPROXY_HPP
#define DIRWALKERCALLBACKPROXY_HPP DIRWALKERCALLBACKPROXY_HPP

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */
#include <Qt>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QObject;
class QFileInfo;

/* forward declaration of local classes */


class DirWalkerCallbackProxy : public DirWalkerCallbacks
{
public:
   DirWalkerCallbackProxy( QObject *target, enum Qt::ConnectionType connectionType = Qt::QueuedConnection );
   virtual ~DirWalkerCallbackProxy();

   /* handle a file entry */
   virtual void handleFile( const QFileInfo &fileInfo );
   /* handle a directory entry upon entering */
   virtual void handleDirEntry( const QFileInfo &fileInfo );
   /* handle a directory entry upon leaving */
   virtual void handleDirLeave( const QFileInfo &fileInfo );
   /* handle an other entry (link, etc.) */
   virtual void handleOther( const QFileInfo &fileInfo );

private:
   QObject                    *mpTarget;
   enum Qt::ConnectionType    mConnectionType;
};

#endif
