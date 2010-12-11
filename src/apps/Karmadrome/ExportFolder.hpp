/**
 * src/apps/Karmadrome/ExportFolder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef EXPORTFOLDER_HPP
#define EXPORTFOLDER_HPP EXPORTFOLDER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QString;

/* forward declaration of local classes */
class Database;



class ExportFolder : public QObject
{
Q_OBJECT

public:
   ExportFolder( const QString &folder, const QString &fileName,
                 bool relative, bool randomize );
   virtual ~ExportFolder();

public slots:
   /* import a playlist from m3u file */
   void writeData( const QStringList &entries );

private:
   ExportFolder( const ExportFolder &that );
   ExportFolder &operator=( const ExportFolder &that );

   bool        mRelative;
   bool        mRandomize;
   QString     mFileName;
};

#endif
