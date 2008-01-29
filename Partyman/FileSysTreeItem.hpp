/**
 * FileSysTreeItem.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSITEM_HPP
#define FILESYSITEM_HPP 1

#include <QList>
#include <QVariant>


class FileSysTreeItem
{
public:
   FileSysTreeItem(const QVariant &data, FileSysTreeItem *parent = 0);
   virtual ~FileSysTreeItem();

   void appendChild(FileSysTreeItem *child);

   FileSysTreeItem *child(int row);
   FileSysTreeItem *child(const QVariant &data);
   int childCount() const;
   QVariant data() const;
   int row() const;
   FileSysTreeItem *parent();

private:
   FileSysTreeItem( const FileSysTreeItem &other );
   FileSysTreeItem &operator=( const FileSysTreeItem &other );

   QList<FileSysTreeItem*> mChildItems;
   QVariant                mItemData;
   FileSysTreeItem         *mpParentItem;
};

#endif
