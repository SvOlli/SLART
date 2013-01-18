/*
 * src/apps/Karmadrome/ExportFolders.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef EXPORTFOLDERLIST_HPP
#define EXPORTFOLDERLIST_HPP EXPORTFOLDERLIST_HPP

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



/*!
  \addtogroup Karmadrome

  @{
*/

/*!
 \brief \todo complete documentation

*/
class ExportFolderList : public QObject
{
   Q_OBJECT

public:
   ExportFolderList( const QString &fileName );
   virtual ~ExportFolderList();

public slots:
   /*!
    \brief 

   */
   void writeData( const QStringList &entries );

signals:

private:
   Q_DISABLE_COPY( ExportFolderList )

   QString     mFileName; /*!< \brief \todo TODO */
};

/*! @} */

#endif
