/*
 * src/apps/Partyman/FileSysTreeItem.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FileSysTreeItem.hpp"

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */


FileSysTreeItem::FileSysTreeItem( const QVariant &data, FileSysTreeItem *parent )
: mpParentItem( parent )
, mItemData( data )
, mChildItems()
{
}


FileSysTreeItem::~FileSysTreeItem()
{
   qDeleteAll(mChildItems);
}


FileSysTreeItem *FileSysTreeItem::child(int row)
{
   return mChildItems.value(row);
}


FileSysTreeItem *FileSysTreeItem::child(const QVariant &data)
{
   int i;

   /* search for item and create it if not found */
   for( i = 0; i < mChildItems.size(); i++ )
   {
      if( data == mChildItems.at(i)->data() )
      {
         return mChildItems.at(i);
      }
      if( data.toString() < mChildItems.at(i)->data().toString() )
      {
         break;
      }
   }

   FileSysTreeItem *fstitem = new FileSysTreeItem( data, this );

   mChildItems.insert( i, fstitem );

   return fstitem;
}


int FileSysTreeItem::childCount() const
{
   return mChildItems.count();
}


QVariant FileSysTreeItem::data() const
{
   return mItemData;
}


FileSysTreeItem *FileSysTreeItem::parent()
{
   return mpParentItem;
}


int FileSysTreeItem::row() const
{
   if (mpParentItem)
   {
      return mpParentItem->mChildItems.indexOf(const_cast<FileSysTreeItem*>(this));
   }

   return 0;
}
