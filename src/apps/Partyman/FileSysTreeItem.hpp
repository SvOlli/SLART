/*
 * src/apps/Partyman/FileSysTreeItem.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FILESYSITEM_HPP
#define FILESYSITEM_HPP FILESYSITEM_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QList>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class FileSysTreeItem
{
public:
   FileSysTreeItem( const QVariant &data, FileSysTreeItem *parent = 0 );
   virtual ~FileSysTreeItem();

   /*!
    \brief return child item by row number

   */
   FileSysTreeItem *child( int row );
   /*!
    \brief return child item by path and create one if none exists

   */
   FileSysTreeItem *child( const QVariant &data );
   /*!
    \brief return number of childs

   */
   int childCount() const;
   /*!
    \brief return path

   */
   QVariant data() const;
   /*!
    \brief return own row id

   */
   int row() const;
   /*!
    \brief return parent

   */
   FileSysTreeItem *parent();

private:
   Q_DISABLE_COPY( FileSysTreeItem )

   FileSysTreeItem         *mpParentItem; /*!< \brief \todo TODO */
   QVariant                mItemData; /*!< \brief \todo TODO */
   QList<FileSysTreeItem*> mChildItems; /*!< \brief \todo TODO */
};

/*! @} */

#endif
