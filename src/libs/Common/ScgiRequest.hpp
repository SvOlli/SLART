/*
 * src/libs/Common/ScgiRequest.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SCGIREQUEST_HPP
#define SCGIREQUEST_HPP SCGIREQUEST_HPP

/* system headers */

/* Qt headers */
#include <QHash>
#include <QByteArray>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class ScgiRequest: public QHash<QByteArray,QByteArray>
{
public:
   /*!
    \brief constructor

    creates the key/value map
    notes:
    - this object is only valid as long as "raw" is valid
    - always create an instance of this object as const

    \param raw raw data of request
   */
   ScgiRequest( const QByteArray &raw );

   /*!
    \brief

    \return const QByteArray
   */
   const QByteArray content() const;

private:
   const char  *mpContentStart; /*!< \brief \todo complete documentation */
   int         mContentSize; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
