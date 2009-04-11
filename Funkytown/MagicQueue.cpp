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
#include <QListWidgetItem>

#include "Trace.hpp"


MagicQueue::MagicQueue()
: mMagicList()
{
}


MagicQueue::~MagicQueue()
{
}


void MagicQueue::addUrl( const QString &addurl, QListWidget *listWidget )
{
   TheMagic *magic = new TheMagic( this );
   magic->mpListWidget = listWidget;
   magic->mURL = addurl;
   magic->mpListWidgetItem = new QListWidgetItem( listWidget );
   addMagic( magic );
}


void MagicQueue::addMagic( TheMagic *magic )
{
   for( int i = 0; i < mMagicList.size(); i++ )
   {
      if( mMagicList.at(i) == magic )
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
         magic->mpListWidgetItem->setText( magic->mURL );
      }
      else
      {
         magic->mpListWidgetItem->setText( magic->mFileName );
      }
   }
   else
   {
      magic->mpListWidgetItem->setText( magic->mMessage );
   }
   
   if( magic->mDownloadToFile )
   {
      magic->mpListWidget->addItem( magic->mpListWidgetItem );
      mMagicList.append( magic );
   }
   else
   {
      magic->mpListWidget->insertItem( 0, magic->mpListWidgetItem );
      mMagicList.prepend( magic );
   }
}


TheMagic *MagicQueue::getMagic()
{
   TheMagic *magic = 0;
   
   if( mMagicList.size() > 0 )
   {
      magic = mMagicList.takeFirst();
      magic->mpListWidget->takeItem( 0 );
   }
   
   return magic;
}

