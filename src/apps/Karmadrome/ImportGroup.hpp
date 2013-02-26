/*
 * src/apps/Karmadrome/ImportGroup.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef IMPORTFOLDER_HPP
#define IMPORTFOLDER_HPP IMPORTFOLDER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class DatabaseInterface;
class TrackInfo;


/*!
  \addtogroup Karmadrome

  @{
*/

/*!
 \brief \todo complete documentation

*/
class ImportGroup : public QObject
{
   Q_OBJECT

public:
   ImportGroup( const QString &folder, const QString &fileName, bool cleanImport );
   virtual ~ImportGroup();

public slots:
   /*!
    \brief 

   */
   void addEntryToGroup( const TrackInfo &ti );

signals:

private:
   Q_DISABLE_COPY( ImportGroup )

   DatabaseInterface *mpDatabase; /*!< \brief \todo TODO */
   int               mQueueSize; /*!< \brief \todo TODO */
   const QString     mFavorite; /*!< \brief \todo TODO */
   const QString     mUnwanted; /*!< \brief \todo TODO */
   QString           mGroup; /*!< \brief \todo TODO */
};

/*! @} */

#endif
