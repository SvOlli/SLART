/**
 * src/apps/Partyman/FileSysTreeModel.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FileSysTreeModel.hpp"

/* system headers */

/* Qt headers */
#include <QMimeData>
#include <QTreeView>
#include <QUrl>

/* local library headers */

/* local headers */
#include "FileSysTreeItem.hpp"


FileSysTreeModel::FileSysTreeModel( QObject *parent )
: QAbstractItemModel( parent )
, mpRootItem(0)
{
   setSupportedDragActions( Qt::CopyAction );
}


FileSysTreeModel::~FileSysTreeModel()
{
   delete mpRootItem;
}


QVariant FileSysTreeModel::data( const QModelIndex &index, int role ) const
{
   if (!index.isValid())
   {
      return QVariant();
   }

   if (role != Qt::DisplayRole)
   {
      return QVariant();
   }

   FileSysTreeItem *item = static_cast<FileSysTreeItem*>( index.internalPointer() );

   return item->data();
}


Qt::ItemFlags FileSysTreeModel::flags( const QModelIndex &index ) const
{
   if (!index.isValid())
   {
      return Qt::ItemIsEnabled;
   }
   if( index.child(0,0).isValid() )
   {
      /* a directory has childs and is not draggable */
      return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
   }
   return Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


Qt::DropActions FileSysTreeModel::supportedDragActions() const
{
   return Qt::CopyAction;
}


QMimeData *FileSysTreeModel::mimeData( const QModelIndexList &indexes ) const
{
   QMimeData *data = new QMimeData();

   QList<QUrl> urls;
   QModelIndex qmi;
   QString fullPath;
   foreach( const QModelIndex &index, indexes )
   {
      fullPath.clear();
      for( qmi = index; qmi.isValid(); qmi = parent(qmi) )
      {
         fullPath.prepend( "/" + qmi.data().toString() );
      }
      urls << QUrl::fromLocalFile( fullPath );
   }
   data->setUrls( urls );

   return data;
}


QStringList FileSysTreeModel::mimeTypes() const
{
   QStringList types;
   types << "text/uri-list" << "text/x-moz-url";;
   return types;
}


#if 0
QVariant FileSysTreeModel::headerData(Qt::Orientation orientation,
                  int role) const
{
   if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
   {
      return mpRootItem->data();
   }

   return QVariant();
}
#endif


QModelIndex FileSysTreeModel::index(int row, int column, const QModelIndex &parent)
       const
{
   FileSysTreeItem *parentItem;

   if (!parent.isValid())
   {
      parentItem =  mpRootItem;
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


QString FileSysTreeModel::getPath( const QModelIndex &index ) const
{
   QModelIndex qmi;
   QString fullPath;

   for( qmi = index; qmi.isValid(); qmi = parent(qmi) )
   {
      fullPath.prepend("/" + qmi.data().toString());
   }

   return fullPath;
}


void FileSysTreeModel::setModelData( const QStringList &fileNames )
{
   clear();

   foreach( const QString &fileName, fileNames )
   {
      addModelData( fileName );
   }
}


void FileSysTreeModel::clear()
{
   if( mpRootItem )
   {
      delete mpRootItem;
      mpRootItem = 0;
   }
   mpRootItem = new FileSysTreeItem( QString("/") );
}


void FileSysTreeModel::addModelData( const QString &filename )
{
   QStringList pathParts( filename.split("/") );
   FileSysTreeItem *fstitem = mpRootItem;

   foreach( const QString &pathPart, pathParts )
   {
      if( pathPart.isEmpty() )
      {
         continue;
      }
      fstitem = fstitem->child( pathPart );
   }
}
