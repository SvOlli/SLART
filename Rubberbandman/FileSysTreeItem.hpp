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
#include <QMutex>

class FileSysTreeItem
{
public:
   FileSysTreeItem( const QVariant &data, FileSysTreeItem *parent = 0 );
   ~FileSysTreeItem();

   void appendChild(FileSysTreeItem *child);

   FileSysTreeItem *child(int row);
#if 0
   FileSysTreeItem *child(const QVariant &data);
#endif
   int childCount();
   QVariant data() const;
   QString fullPath() const;
   int row() const;
   FileSysTreeItem *parent();

private:
   static QMutex           mMutex;
   FileSysTreeItem( const FileSysTreeItem &other );
   FileSysTreeItem &operator=( const FileSysTreeItem &other );

   FileSysTreeItem         *mpParentItem;
   QVariant                mItemData;
   QList<FileSysTreeItem*> mChildItems;
   int                     mScanned;
};

#endif
