/**
 * src/apps/Karmadrome/ImportExport.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef IMPORTEXPORT_HPP
#define IMPORTEXPORT_HPP IMPORTEXPORT_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QString;

/* forward declaration of local classes */
class Database;



class ImportExport
{

public:
   ImportExport( Database *database );

public:
   /* import a playlist from m3u file */
   void importM3u( QString folder, const QString &fileName, bool cleanImport );
   /* export a playlist to m3u file */
   void exportM3u( QString folder, const QString &fileName, bool relative, bool randomize );
   /* print out a list of all folders */
   void listFolders( const QString &fileName );

private:
   ImportExport( const ImportExport &that );
   ImportExport &operator=( const ImportExport &that );

   Database        *mpDatabase;
};

#endif
