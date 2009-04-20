/**
 * MagicQueue.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MagicQueue.hpp"

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "TheMagic.hpp"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QListWidget>
#include <QListWidgetItem>

#include "Trace.hpp"

#define USE_TRACE 0


MagicQueue::MagicQueue( QWidget *parent )
: QListWidget( parent )
, mMagicList()
{
   connect( this, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(handleSelect(QListWidgetItem*)) );
}


MagicQueue::~MagicQueue()
{
}


void MagicQueue::addUrl( const QString &addurl )
{
   TheMagic *magic = new TheMagic( this );
   magic->mURL = addurl;
   addMagic( magic );
}


void MagicQueue::addMagic( TheMagic *magic )
{
#if USE_TRACE
TRACESTART(MagicQueue::addMagic)
#endif
   for( int i = 0; i < mMagicList.size(); i++ )
   {
      if( mMagicList.at(i)->mURL == magic->mURL )
      {
         /* already in list, no need to enqueue */
         delete magic;
         return;
      }
   }
   
   if( magic->mMessage.isEmpty() )
   {
      if( magic->mFileName.isEmpty() )
      {
         magic->mMessage = magic->mURL;
      }
      else
      {
         magic->mMessage = magic->mFileName;
      }
   }
   
   QListWidgetItem *qlwi = 0;
   if( magic->mFileName.isEmpty() )
   {
      int i;
      magic->mDownloadToFile = false;
      for( i = 0; i < count(); i++ )
      {
         if( !(mMagicList.at(i)->mFileName.isEmpty()) )
         {
            break;
         }
      }
      insertItem( i, magic->mMessage );
      mMagicList.insert( i, magic );
      qlwi = item( i );
   }
   else
   {
      magic->mDownloadToFile = true;
      addItem( magic->mMessage );
      mMagicList.append( magic );
      qlwi = item( count() - 1 );
   }
   qlwi->setSelected( magic->mSelected );
#if LIST_NEEDS_FIXING
   updateList();
#endif
}


TheMagic *MagicQueue::getMagic()
{
#if USE_TRACE
TRACESTART(MagicQueue::getMagic)
#endif
   TheMagic *magic = 0;
   
   if( mMagicList.size() > 0 )
   {
#if USE_TRACE
      QListWidgetItem *item = takeItem( 0 );
      magic = mMagicList.takeFirst();
      if( item->text() != magic->mMessage )
      {
TRACEMSG << item->text() << "!=" << magic->mMessage;
      }
#else
      takeItem( 0 );
      magic = mMagicList.takeFirst();
#endif
   }
   
#if LIST_NEEDS_FIXING
   updateList();
#endif
   return magic;
}


#if LIST_NEEDS_FIXING
void MagicQueue::updateList()
{
   if( mMagicList.count() != count() )
   {
      clear();
      for( int i = 0; i < mMagicList.count(); i++ )
      {
         addItem( mMagicList.at(i)->mMessage );
      }
   }
}
#endif


void MagicQueue::handleSelect( QListWidgetItem *item )
{
   TheMagic *magic = mMagicList.at( row( item ) );
   magic->mSelected = item->isSelected();
}
