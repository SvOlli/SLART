/*
 * src/apps/Rubberbandman/DirWalkerCallbackProxy.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERCALLBACKPROXY_HPP
#define DIRWALKERCALLBACKPROXY_HPP DIRWALKERCALLBACKPROXY_HPP

/* base class */
#include "DirWalker.hpp"

/* system headers */

/* Qt headers */
#include <Qt>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QObject;
class QFileInfo;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 unused

 \dotfile "graphs/libs/Common/DirWalkerCallbackProxy_connect.dot" "Connect Graph"
*/
class DirWalkerCallbackProxy : public DirWalkerCallbacks
{
public:
   DirWalkerCallbackProxy( QObject *target,
                           Qt::ConnectionType connectionType = Qt::QueuedConnection );
   /*!
    \brief \todo

   */
   /*!
    \brief \todo

   */
   virtual ~DirWalkerCallbackProxy();

   /*!
    \brief called on start

   */
   virtual void handleStart();
   /*!
    \brief handle a file entry

   */
   virtual void handleFile( const QFileInfo &fileInfo );
   /*!
    \brief handle a directory entry upon entering

   */
   virtual void handleDirEntry( const QFileInfo &fileInfo );
   /*!
    \brief handle a directory entry upon leaving

   */
   virtual void handleDirLeave( const QFileInfo &fileInfo );
   /*!
    \brief handle an other entry (link, etc.)

   */
   virtual void handleOther( const QFileInfo &fileInfo );
   /*!
    \brief called on start

   */
   virtual void handleEnd();

private:
   QObject                    *mpTarget;
   enum Qt::ConnectionType    mConnectionType;
};

/*! @} */

#endif
