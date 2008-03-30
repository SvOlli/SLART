/**
 * FileSysTreeItem.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSITEM_HPP
#define FILESYSITEM_HPP FILESYSITEM_HPP

#include <QList>
#include <QVariant>


class FileSysTreeItem
{
public:
   FileSysTreeItem( const QVariant &data, FileSysTreeItem *parent = 0 );
   virtual ~FileSysTreeItem();

   /* return child item by row number */
   FileSysTreeItem *child( int row );
   /* return child item by path and create one if none exists */
   FileSysTreeItem *child( const QVariant &data );
   /* return number of childs */
   int childCount() const;
   /* return path */
   QVariant data() const;
   /* return own row id */
   int row() const;
   /* return parent */
   FileSysTreeItem *parent();

private:
   FileSysTreeItem( const FileSysTreeItem &other );
   FileSysTreeItem &operator=( const FileSysTreeItem &other );

   FileSysTreeItem         *mpParentItem;
   QVariant                mItemData;
   QList<FileSysTreeItem*> mChildItems;
};

#endif
