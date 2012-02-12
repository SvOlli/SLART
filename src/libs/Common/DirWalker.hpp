/*
 * src/libs/Common/DirWalker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DIRWALKER_HPP
#define DIRWALKER_HPP DIRWALKER_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QObject>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;
class QString;

/* forward declaration of local classes */


/*!
    \brief interface to provide callbacks

   */
/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class DirWalkerCallbacks
{
public:
   /*!
    \brief must be virtual because of virtual functions

   */
   virtual ~DirWalkerCallbacks(){}
   /*!
    \brief called on start

   */
   virtual void handleStart()                               = 0;
   /*!
    \brief handle a file entry

   */
   virtual void handleFile( const QFileInfo &fileInfo )     = 0;
   /*!
    \brief handle a directory entry upon entering

   */
   virtual void handleDirEntry( const QFileInfo &fileInfo ) = 0;
   /*!
    \brief handle a directory entry upon leaving

   */
   virtual void handleDirLeave( const QFileInfo &fileInfo ) = 0;
   /*!
    \brief handle an other entry (link, etc.)

   */
   virtual void handleOther( const QFileInfo &fileInfo )    = 0;
   /*!
    \brief called on end

   */
   virtual void handleEnd()                                 = 0;
};

/*! @} */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class DirWalker
{
public:
   DirWalker();
   virtual ~DirWalker();

   /*!
    \brief let the directory walker travel the filesystem

   */
   void run( DirWalkerCallbacks *callbacks,
             const QString &directoryPath );

private:
   Q_DISABLE_COPY( DirWalker )

   /*!
    \brief let the directory walker travel the filesystem

   */
   void runDir( DirWalkerCallbacks *callbacks,
                const QString &directoryPath );

};

/*! @} */

#endif
