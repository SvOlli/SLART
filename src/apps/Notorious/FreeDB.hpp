/*
 * src/apps/Notorious/FreeDB.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FREEDB_HPP
#define FREEDB_HPP FREEDB_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QSqlDatabase;

/* forward declaration of local classes */
class FreeDBQuery;


/*!
  \addtogroup Notorious

  @{
*/

/*!
 \brief \todo complete documentation

*/
class FreeDB : public QObject
{
   Q_OBJECT

public:
   FreeDB( QObject *parent = 0 );
   virtual ~FreeDB();

   static QStringList Categories();

private:
   Q_DISABLE_COPY( FreeDB )

   QSqlDatabase *mpSqlDB; /*!< \brief \todo TODO */
   FreeDBQuery  *mpSearchQuery; /*!< \brief \todo TODO */
   FreeDBQuery  *mpInfoQuery; /*!< \brief \todo TODO */
};

/*! @} */


#endif
