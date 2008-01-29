/**
 * FileSysTreeItem.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysTreeItem.hpp"
#include "Trace.hpp"

#include <QVariant>
#include <QDir>
#include <QString>
#include "Trace.hpp"


QMutex FileSysTreeItem::mMutex;

FileSysTreeItem::FileSysTreeItem(const QVariant &data, FileSysTreeItem *parent)
: mpParentItem( parent )
, mItemData( data )
, mScanned( -1 )
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


#if 0
FileSysTreeItem *FileSysTreeItem::child(const QVariant &data)
{
   /* search for item and create it if not found */
   for( int i = 0; i < mChildItems.size(); i++ )
   {
      if( data == mChildItems.at(i)->data() )
      {
         return mChildItems.at(i);
      }
   }
   
   FileSysTreeItem *fstitem = new FileSysTreeItem( mPath, data, this );
   mChildItems.append( fstitem );
   return fstitem;
}
#endif


int FileSysTreeItem::childCount()
{
   if( mScanned < 0 )
   {
      QString path( mItemData.toString() );
      
      QDir dir;
      if( dir.cd( path ) )
      {
         
         QStringList filters;
         filters << "*.ogg";
         
         QStringList entries( dir.entryList( //filters, 
            QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot,
            QDir::Name | QDir::DirsFirst ) );
         
         mChildItems.clear();
         
         QStringListIterator iterator( entries );
         while (iterator.hasNext())
         {
            QString entry(iterator.next());
            
            FileSysTreeItem *fstitem = new FileSysTreeItem( QVariant(path+"/"+entry), this );
            mChildItems.append( fstitem );
         }
         mScanned = mChildItems.count();
      }
      else
      {
         mScanned = 0;
      }
   }
   return mScanned;
}


QVariant FileSysTreeItem::data() const
{
   QString basename = mItemData.toString();
   basename.remove( QRegExp( "^.*/" ) );
   return QVariant(basename);
}


QString FileSysTreeItem::fullPath() const
{
   return mItemData.toString();
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

