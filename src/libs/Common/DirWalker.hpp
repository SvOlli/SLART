/**
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

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;
class QString;

/* forward declaration of local classes */


/* interface to provide callbacks */
class DirWalkerCallbacks
{
public:
   /* must be virtual because of virtual functions */
   virtual ~DirWalkerCallbacks(){}
   /* handle a file entry */
   virtual void handleFile( const QFileInfo &fileInfo )     = 0;
   /* handle a directory entry upon entering */
   virtual void handleDirEntry( const QFileInfo &fileInfo ) = 0;
   /* handle a directory entry upon leaving */
   virtual void handleDirLeave( const QFileInfo &fileInfo ) = 0;
   /* handle an other entry (link, etc.) */
   virtual void handleOther( const QFileInfo &fileInfo )    = 0;
};


class DirWalker
{
public:
   DirWalker();
   virtual ~DirWalker();

   /* let the directory walker travel the filesystem */
   void run( DirWalkerCallbacks *callbacks,
             const QString &directoryPath );

private:
   DirWalker( const DirWalker &that );
   DirWalker &operator=( const DirWalker &that );
};

#endif
