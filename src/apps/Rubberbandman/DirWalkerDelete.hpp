/**
 * src/apps/Rubberbandman/DirWalkerDelete.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERDELETE_HPP
#define DIRWALKERDELETE_HPP DIRWALKERDELETE_HPP

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;

/* forward declaration of local classes */
class DatabaseInterface;


class DirWalkerDelete : public DirWalkerCallbacks
{
public:
   DirWalkerDelete();
   virtual ~DirWalkerDelete();

   /* handle a file entry */
   virtual void handleFile( const QFileInfo &fileInfo );
   /* handle a directory entry upon entering */
   virtual void handleDirEntry( const QFileInfo &fileInfo );
   /* handle a directory entry upon leaving */
   virtual void handleDirLeave( const QFileInfo &fileInfo );
   /* handle an other entry (link, etc.) */
   virtual void handleOther( const QFileInfo &fileInfo );

private:
   DatabaseInterface    *mpDatabase;
   QDir                 mDir;
};

#endif
