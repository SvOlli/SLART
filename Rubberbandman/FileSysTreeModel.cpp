/**
 * FileSysTreeModel.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysTreeModel.hpp"
#include "FileSysTreeItem.hpp"

#include <QtDebug>
#include <QTreeView>


FileSysTreeModel::FileSysTreeModel( const QString &rootPath, QObject *parent )
: QAbstractItemModel( parent )
, mpRootItem( new FileSysTreeItem( QVariant(rootPath) ) )
, mRootPath( rootPath )
{
   mRootPath.append("/");
}


FileSysTreeModel::~FileSysTreeModel()
{
   delete mpRootItem;
}


QVariant FileSysTreeModel::data(const QModelIndex &index, int role) const
{
   if (!index.isValid())
   {
      return QVariant();
   }

   if (role != Qt::DisplayRole)
   {
      return QVariant();
   }

   FileSysTreeItem *item = static_cast<FileSysTreeItem*>(index.internalPointer());

   return item->data();
}


Qt::ItemFlags FileSysTreeModel::flags(const QModelIndex &index) const
{
   if (!index.isValid())
   {
      return Qt::ItemIsEnabled;
   }

   return Qt::ItemIsEnabled /*| Qt::ItemIsSelectable*/;
}


QVariant FileSysTreeModel::headerData(Qt::Orientation orientation,
                  int role) const
{
   if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
   {
      return mpRootItem->data();
   }

   return QVariant();
}


QModelIndex FileSysTreeModel::index(int row, int column, const QModelIndex &parent)
       const
{
   FileSysTreeItem *parentItem;

   if (!parent.isValid())
   {
      parentItem = mpRootItem;
   }
   else
   {
      parentItem = static_cast<FileSysTreeItem*>(parent.internalPointer());
   }

   FileSysTreeItem *childItem = parentItem->child(row);
   if (childItem)
   {
      return createIndex(row, column, childItem);
   }
   else
   {
      return QModelIndex();
   }
}


QModelIndex FileSysTreeModel::parent(const QModelIndex &index) const
{
   if (!index.isValid())
   {
      return QModelIndex();
   }

   FileSysTreeItem *childItem = static_cast<FileSysTreeItem*>(index.internalPointer());
   FileSysTreeItem *parentItem = childItem->parent();

   if (parentItem == mpRootItem)
   {
      return QModelIndex();
   }

   return createIndex(parentItem->row(), 0, parentItem);
}


int FileSysTreeModel::rowCount(const QModelIndex &parent) const
{
   FileSysTreeItem *parentItem;

   if (!parent.isValid())
   {
      parentItem = mpRootItem;
   }
   else
   {
      parentItem = static_cast<FileSysTreeItem*>(parent.internalPointer());
   }

   return parentItem->childCount();
}


int FileSysTreeModel::columnCount(const QModelIndex &) const
{
   return 1;
}


QString FileSysTreeModel::getPath( const QModelIndex & index )
{
   QModelIndex qmi;
   QString fullPath;
   
   for( qmi = index; qmi.isValid(); qmi = parent(qmi) )
   {
      fullPath.prepend(qmi.data().toString() + "/");
   }
   fullPath.chop(1);
   fullPath.prepend(mRootPath);
   
   return fullPath;
}


void FileSysTreeModel::currentChanged( const QModelIndex & current,
                                       const QModelIndex & previous )
{
   emit activated(current);
}
