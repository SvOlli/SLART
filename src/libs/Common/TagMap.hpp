/*
 * src/libs/Common/TagMap.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TAGMAP_HPP
#define TAGMAP_HPP TAGMAP_HPP

/* base class */
#include <QMap>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */
#include <MyLua.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 constructors and destructors are not explicitly created on purpose in favour
 of reusing the ones from the base class
*/
class TagMap: public QMap<QByteArray,QString>
{
public:
   /*!
    \brief generate filename according to tags

   */
   QString fileName( const QString &pattern, bool filterPath = true );
   /*!
    \brief normalize a string

   */
   static QString normalizeString( const QString &string );
   /*!
    \brief convert to lua.table

   */
   MyLuaTable toLuaTable();

private:
};

/*! @} */

#endif
