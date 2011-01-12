/**
 * src/apps/Rubberbandman/DirWalkerUpdate.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DIRWALKERUPDATE_HPP
#define DIRWALKERUPDATE_HPP DIRWALKERUPDATE_HPP

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;

/* forward declaration of local classes */
class DatabaseWorker;


class DirWalkerUpdate : public DirWalkerCallbacks
{
public:
   DirWalkerUpdate( DatabaseWorker *databaseWorker );
   virtual ~DirWalkerUpdate();

   /* handle a file entry */
   virtual void handleFile( const QFileInfo &fileInfo );
   /* handle a directory entry upon entering */
   virtual void handleDirEntry( const QFileInfo &fileInfo );
   /* handle a directory entry upon leaving */
   virtual void handleDirLeave( const QFileInfo &fileInfo );
   /* handle an other entry (link, etc.) */
   virtual void handleOther( const QFileInfo &fileInfo );

private:
   DatabaseWorker    *mpDatabaseWorker;
};

#endif
