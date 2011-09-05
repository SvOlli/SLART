/**
 * src/libs/Common/TagList.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TAGLIST_HPP
#define TAGLIST_HPP TAGLIST_HPP

#include <QStringList>


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class TagList
{
public:
   TagList();
   TagList &operator=( const TagList &that );

   /*!
    \brief return number of tags

   */
   int count() const;
   /*!
    \brief clear all tags

   */
   void clear();
   /*!
    \brief read tag name

   */
   QString tagAt( int i ) const;
   /*!
    \brief read tag value

   */
   QString valueAt( int i ) const;
   /*!
    \brief set/add tag

   */
   void set( const QString &tag, const QString &value = QString() );
   /*!
    \brief get a tag

   */
   QString get( const QString &tag );
   /*!
    \brief generate filename according to tags

   */
   QString fileName( const QString &pattern, bool filterPath = true );
   /*!
    \brief normalize a string

   */
   static QString normalizeString( const QString &string );
   /*!
    \brief normalize a tag

   */
   QString normalizeTag( const QString &tag );

private:
   TagList( const TagList &that );

   QStringList mTags;
   QStringList mValues;
};

/*! @} */

#endif
