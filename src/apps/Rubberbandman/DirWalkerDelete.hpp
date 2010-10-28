/**
 * src/apps/Rubberbandman/DirWalkerDelete.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* base class */
#include <DirWalker.hpp>

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QString;
class QFileInfo;

/* forward declaration of local classes */
class Database;


class DirWalkerDelete : public DirWalkerCallbacks
{
public:
   DirWalkerDelete( Database *database/*, const QString &path*/ );
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
   Database    *mpDatabase;
   QDir        mDir;
   TrackInfo   mTrackInfo;
};
