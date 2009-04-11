/**
 * TheMagic.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "TheMagic.hpp"

#include "ConfigDialog.hpp"
#include "MagicQueue.hpp"
#include "MySettings.hpp"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QListWidgetItem>

#include "Trace.hpp"


TheMagic::TheMagic( MagicQueue *magicQueue )
: QObject()
, mURL()
, mStage( stageFresh )
, mpMagicQueue( magicQueue )
, mDownloadToFile( false )
, mMessage()
, mBuffer()
, mpFile( 0 )
, mpBuffer( 0 )
, mpListWidget( 0 )
, mpListWidgetItem( 0 )
, mMySpaceArtId()
, mMySpaceProfId()
, mMySpacePlaylistId()
, mMySpaceSongId()
{
}


TheMagic::~TheMagic()
{
   if( mpFile )
   {
      delete mpFile;
   }
   if( mpBuffer )
   {
      delete mpBuffer;
   }
   if( mpListWidgetItem )
   {
      delete mpListWidgetItem;
   }
}


/*
 * Object that should not be copied from one object to another:
 * - mpFile
 * - mpBuffer
 * - mpListWidgetItem
 */

TheMagic::TheMagic( const TheMagic &other )
: QObject()
, mURL              ( other.mURL )
, mStage            ( other.mStage )
, mpMagicQueue      ( other.mpMagicQueue )
, mDownloadToFile   ( other.mDownloadToFile )
, mMessage          ( other.mMessage )
, mBuffer           ( other.mBuffer )
, mpFile            ( 0 )
, mpBuffer          ( 0 )
, mpListWidget      ( other.mpListWidget )
, mpListWidgetItem  ( new QListWidgetItem( mpListWidget ) )
, mMySpaceArtId     ( other.mMySpaceArtId )
, mMySpaceProfId    ( other.mMySpaceProfId )
, mMySpacePlaylistId( other.mMySpacePlaylistId )
, mMySpaceSongId    ( other.mMySpaceSongId )
{
   mpListWidgetItem->setSelected( other.mpListWidgetItem->isSelected() );
}


TheMagic &TheMagic::operator=( const TheMagic &other )
{
TRACESTART(TheMagic::operator=)
   mURL               = other.mURL;
   mStage             = other.mStage;
   mpMagicQueue       = other.mpMagicQueue;
   mDownloadToFile    = other.mDownloadToFile;
   mMessage           = other.mMessage;
   mBuffer            = other.mBuffer;
   mpFile             = 0;
   mpBuffer           = 0;
   mpListWidget       = other.mpListWidget;
   mpListWidgetItem   = new QListWidgetItem( mpListWidget );
   mMySpaceArtId      = other.mMySpaceArtId;
   mMySpaceProfId     = other.mMySpaceProfId;
   mMySpacePlaylistId = other.mMySpacePlaylistId;
   mMySpaceSongId     = other.mMySpaceSongId;
   
   mpListWidgetItem->setSelected( other.mpListWidgetItem->isSelected() );
   
   return *this;
}


bool TheMagic::operator==( const TheMagic &other ) const
{
   return (mURL == other.mURL) && (mStage == other.mStage);
}


QListWidgetItem *TheMagic::listWidgetItem()
{
   return mpListWidgetItem;
}


const QString &TheMagic::message()
{
   return mMessage;
}
 
 
QString TheMagic::fileName()
{
   if( mDownloadToFile )
   {
      sanitizeFileName();
      return mFileName;
   }
   else
   {
      return tr("*Buffer*");
   }
}


QIODevice *TheMagic::ioDevice()
{
   if( mDownloadToFile )
   {
      return static_cast<QIODevice*>(mpFile);
   }
   else
   {
      return static_cast<QIODevice*>(mpBuffer);
   }
}


void TheMagic::downloadToFile()
{
   sanitizeFileName();
   mpFile = new QFile( mFileName );
   
   if( 1/* check for do not overwrite */ )
   {
      if( mpFile->exists() )
      {
         delete mpFile;
         mpFile = 0;
      }
   }
   
   if( mpFile && !(mpFile->open( QIODevice::WriteOnly )) )
   {
      delete mpFile;
      mpFile = 0;
   }
   
   mDownloadToFile = true;
}


void TheMagic::downloadToBuffer()
{
   mpBuffer = new QBuffer;
   
   mpBuffer->open(QBuffer::WriteOnly);
   
   mDownloadToFile = false;
}


void TheMagic::downloadClose()
{
   if( mpFile )
   {
      mpFile->close();
      if( !mSuccess )
      {
         mpFile->remove();
      }
      delete mpFile;
      mpFile = 0;
   }
   
   if( mpBuffer )
   {
      mBuffer = QString::fromLatin1( mpBuffer->buffer() );
      mpBuffer->close();
      delete mpBuffer;
      mpBuffer = 0;
   }
}
 


void TheMagic::sanitizeFileName()
{
   mFileName.remove( QRegExp("[\\\\:?]") );
   mFileName.replace( QRegExp("[\"|/\\*]"), "_" ); 
}


QString TheMagic::xmlParam( const QString &string, int pos )
{
   const QChar quot('"');
   int q1 = string.indexOf( quot, pos );
   int q2 = string.indexOf( quot, q1+1 );
   return string.mid(q1+1,q2-q1-1);
}


void TheMagic::preDownload()
{
   switch( mStage )
   {
      case stageFresh:
         parseURL();
         break;
      case stageGenericFile:
      case stageMySpaceMP3:
         downloadToFile();
         break;
      case stageGenericHTML:
      case stageMySpaceHTML:
      case stageMySpaceOldXML:
      case stageMySpacePlaylists:
      case stageMySpaceArtistPlaylist:
      case stageMySpaceSong:
      case stageYouTubeHTML:
         downloadToBuffer();
         break;
      default:
         break;
   }
}


void TheMagic::postDownload( bool success )
{
   mSuccess = success;
   downloadClose();
   switch( mStage )
   {
      case stageFresh:
         break;
      case stageGenericFile:
         processGenericFile();
         break;
      case stageGenericHTML:
         parseGenericHTML();
         break;
      case stageMySpaceHTML:
         parseMySpaceHTML();
         break;
      case stageYouTubeHTML:
         parseYouTubeHTML();
         break;
      case stageMySpaceOldXML:
         parseMySpaceOldXML();
         break;
      case stageMySpaceMP3:
         parseMySpaceMP3();
         break;
      case stageMySpacePlaylists:
         parseMySpacePlaylists();
         break;
      case stageMySpaceArtistPlaylist:
         parseMySpaceArtistPlaylist();
         break;
      case stageMySpaceSong:
         parseMySpaceSong();
         break;
      default:
         break;
   }
}


void TheMagic::parseURL()
{
   if( mURL.contains( QRegExp( "(.mp3|.ogg|.m4a)$", Qt::CaseInsensitive, QRegExp::RegExp2 ) ) )
   {
      mStage = stageGenericFile;
      downloadToFile();
   }
   else if( mURL.contains( QRegExp( "(.flv|.mp4)$", Qt::CaseInsensitive, QRegExp::RegExp2 ) ) )
   {
      mStage = stageGenericFile;
      downloadToFile();
   }
   else if( mURL.contains( QRegExp( "^http://[^/]*myspace.com/", Qt::CaseInsensitive, QRegExp::RegExp2 ) ) )
   {
      mStage = stageMySpaceHTML;
      downloadToBuffer();
   }
   else if( mURL.contains( QRegExp( "^http://[^/]*youtube.com/", Qt::CaseInsensitive, QRegExp::RegExp2 ) ) )
   {
      mStage = stageYouTubeHTML;
      downloadToBuffer();
   }
   else
   {
      mStage = stageGenericHTML;
      downloadToBuffer();
   }
}


void TheMagic::processGenericFile()
{
   QFileInfo fileinfo( mFileName );
   if( mSuccess && (fileinfo.size() > 0) )
   {
      MySettings settings;
      qulonglong total = settings.VALUE_BYTES;
      total += fileinfo.size();
      settings.setValue( "Bytes", total );
      
      unsigned int count = settings.VALUE_FILES + 1;
      settings.setValue( "Files", count );
      
      settings.sendNotification( QString("f0v\n") + 
                                 QDir::currentPath() + '/' + mFileName );
   }
}


void TheMagic::parseGenericHTML()
{
   int pos;
   QString param("http://www.youtube.com/v/");
   QString embed;
   QString line;
   QRegExp href( "href=.*mp3", Qt::CaseInsensitive );
   
   QStringList qsl( mBuffer.split( QChar('\n') ) );
   for( int i = 0; i < qsl.size(); i++ )
   {
      line = qsl.at(i);
      pos = line.indexOf( param, 0, Qt::CaseInsensitive );
      if( pos > 0 )
      {
         QString embed( line );
         embed.replace( QRegExp(".*\"http://"), "http://" );
         embed.remove( QRegExp("&.*") );
         embed.replace( "/v/", "/watch?v=" );
         
         TheMagic *magic = new TheMagic( *this );
         magic->mURL   = embed;
         magic->mStage = stageYouTubeHTML;
         
         mpMagicQueue->addMagic( magic );
      }
      pos = line.indexOf( href );
      if( pos > 0 )
      {
         QString mp3ref( line );
         mp3ref.remove( QRegExp("^.*href=['\"]*") );
         mp3ref.remove( QRegExp("\\.mp3['\"]*.*$") );
         mp3ref.append( ".mp3" );
         
         if( !mp3ref.startsWith( "http://" ) )
         {
            if( mp3ref.startsWith( "/" ) )
            {
               mp3ref.prepend( mURL.left( mURL.indexOf( '/', 8 ) ) );
            }
            else
            {
               mp3ref.prepend( mURL.left( mURL.lastIndexOf( '/' ) + 1 ) );
            }
         }
         QString filename( mp3ref );
         filename.remove( QRegExp( "^.*/" ) );
         
         TheMagic *magic = new TheMagic( *this );
         magic->mURL   = mp3ref;
         magic->mStage = stageGenericFile;
         
         mpMagicQueue->addMagic( magic );
      }
   }
}


void TheMagic::parseYouTubeHTML()
{
   int pos;
   QString line;
   QString youTube("fullscreenUrl");
   QString t;
   QString title;
   QString videoID;
   
   QStringList qsl( mBuffer.split( QChar('\n') ) );
   for( int i = 0; i < qsl.size(); i++ )
   {
      line = qsl.at(i);
      pos = line.indexOf( youTube, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         QStringList parts( line.split( "&" ) );
         for( pos = 0; pos < parts.count(); pos++ )
         {
            if( parts.at(pos).startsWith( "t=" ) )
            {
               t=parts.at(pos);
            }
            
            if( parts.at(pos).startsWith( "video_id=" ) )
            {
               videoID=parts.at(pos);
            }
            
            if( parts.at(pos).startsWith( "title=" ) )
            {
               title=parts.at(pos).left(parts.at(pos).lastIndexOf("'")).mid(6);
            }
         }
      }
   }
   
   if( !(videoID.isEmpty()) && !(t.isEmpty()) && !(title.isEmpty()) )
   {
      TheMagic *magic = new TheMagic( *this );
      magic->mURL = QString("http://youtube.com/get_video?")+videoID+"&"+t;
      magic->mFileName = title+QString(".flv");
      magic->mStage = stageGenericFile;
      mpMagicQueue->addMagic( magic );
   }
}


void TheMagic::parseMySpaceHTML()
{
   int           pos;
   int           endPos;
   const QString profid("&profid=");
   const QString artid("&artid=");
   const QChar   ampersand('&');
   //QString line;
   
   QStringList qsl( mBuffer.split( QChar('\n') ) );
   for( int i = 0; i < qsl.size(); i++ )
   {
      const QString line( qsl.at(i) );
      
      pos = line.indexOf( artid, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         pos += artid.size();
         endPos = line.indexOf( ampersand, pos );
         if( endPos >= 0 )
         {
            mMySpaceArtId = line.mid( pos, endPos-pos );
         }
      }
      
      pos = line.indexOf( profid, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         pos += profid.size();
         endPos = line.indexOf( ampersand, pos );
         if( endPos >= 0 )
         {
            mMySpaceProfId = line.mid( pos, endPos-pos );
         }
      }
   }

   if( !mMySpaceProfId.isEmpty() )
   {
      TheMagic *magic = new TheMagic( *this );
      magic->mURL = QString("http://mediaservices.myspace.com/services/media/musicplayerxml.ashx?b=");
      magic->mURL.append( mMySpaceProfId );
      magic->mStage = stageMySpaceOldXML;
      mpMagicQueue->addMagic( magic );
      
      if( !mMySpaceArtId.isEmpty() )
      {
         TheMagic *magic = new TheMagic( *this );
         magic->mURL = QString("http://musicservices.myspace.com/Modules/MusicServices/Services/MusicPlayerService.ashx?"
                               "action=getPlaylists&friendId=");
         magic->mURL.append( mMySpaceProfId );
         magic->mStage = stageMySpacePlaylists;
         mpMagicQueue->addMagic( magic );
      }
   }
}


void TheMagic::parseMySpaceOldXML()
{
   int i;
   bool quoton = false;
   bool cdataon = false;
   
   for( i = 0; i < mBuffer.size(); i++ )
   {
      if( mBuffer.mid(i,8) == QString("![CDATA[") )
      {
         cdataon = true;
      }
      switch(mBuffer.at(i).toAscii() )
      {
         case '"':
            quoton = !quoton;
            break;
         case ']':
            if( cdataon ) cdataon = false;
            break;
         case ' ':
         case '<':
         case '>':
         case '\r':
            if( !quoton && !cdataon )
            {
               mBuffer.replace(i,1,'\n');
            }
            break;
         default:
            // nothing
            break;
      }
   }
   
   QStringList dataList = mBuffer.split( "\n", QString::SkipEmptyParts );
   QString xmlName;
   QString xmlTitle;
   QString xmlUrl;
   for( i = 0; i < dataList.size(); i++ )
   {
      if( dataList.at(i) == QString("name") )
      {
         xmlName = dataList.at(i+1);
         xmlName.replace( QRegExp( "!\\[CDATA\\[(.*)\\]\\]" ), "\\1" );
         //TRACEMSG << "name" << name;
      }
      if( dataList.at(i) == QString("song") )
      {
         xmlTitle = "";
         xmlUrl   = "";
         //TRACEMSG << "song";
      }
      if( dataList.at(i).startsWith("title=") )
      {
         xmlTitle = dataList.at(i);
         xmlTitle.replace( QRegExp( ".*\"(.*)\".*" ), "\\1" );
         //TRACEMSG << "title" << title;
      }
      if( dataList.at(i).startsWith("curl=") ||
          dataList.at(i).startsWith("durl=") || 
          dataList.at(i).startsWith("purl=") || 
          dataList.at(i).startsWith("downloadable=") )
      {
         xmlUrl = dataList.at(i);
         if( xmlUrl.indexOf(".mp3") < 0 )
         {
            xmlUrl.clear();
         }
         else
         {
            xmlUrl.replace( QRegExp( ".*\"(.*)[\\?\"].*" ), "\\1" );
            xmlUrl.replace("std_","full_");
            xmlUrl.replace("&amp;","&");
         }
      }
      if( !xmlUrl.isEmpty() && !xmlTitle.isEmpty() )
      {
         TheMagic *magic = new TheMagic( *this );
         magic->mURL = xmlUrl;
         magic->mFileName = xmlName+QString(" - ")+xmlTitle+QString(".mp3");
         magic->mStage = stageMySpaceMP3;
         mpMagicQueue->addMagic( magic );
         
         xmlTitle = "";
         xmlUrl   = "";
      }
   }
}


void TheMagic::parseMySpaceMP3()
{
   QString full_("/full_");
   
   QFileInfo fileinfo( mFileName );
   if( !(fileinfo.size()) )
   {
      mSuccess = false;
   }
   
   if( mSuccess )
   {
      MySettings settings;
      qulonglong total = settings.VALUE_BYTES;
      total += fileinfo.size();
      settings.setValue( "Bytes", total );
      
      unsigned int count = settings.VALUE_FILES + 1;
      settings.setValue( "Files", count );
      
      settings.sendNotification( QString("f0d\n") + 
                                 QDir::currentPath() + '/' + mFileName );
      if( mpListWidgetItem->isSelected() )
      {
         settings.sendUdpMessage( QString("P0Q\n") + 
                                  QDir::currentPath() + '/' + mFileName, QString("Partyman") );
      }
   }
   else
   {
      if( mURL.indexOf( full_ ) >= 0 )
      {
         QString std_("/std_");
         
         TheMagic *magic = new TheMagic( *this );
         magic->mURL.replace( full_, std_ );
         mpMagicQueue->addMagic( magic );
      }
   }
}


void TheMagic::parseMySpacePlaylists()
{
   int pos;
   const QString playlistId("playlistId=");
   
   QStringList qsl( mBuffer.split( QRegExp("[<>]") ) );
   for( int i = 0; i < qsl.size(); i++ )
   {
      const QString line( qsl.at(i) );
      
      pos = line.indexOf( playlistId, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         mMySpacePlaylistId = xmlParam( qsl.at(i), pos );
         
         TheMagic *magic = new TheMagic( *this );
         magic->mURL = QString("http://musicservices.myspace.com/Modules/MusicServices/Services/MusicPlayerService.ashx?"
                               "action=getArtistPlaylist&artistId=");
         magic->mURL.append( mMySpaceArtId );
         magic->mURL.append( "&playlistId=" );
         magic->mURL.append( mMySpacePlaylistId );
         magic->mURL.append( "&artistUserId=" );
         magic->mURL.append( mMySpaceProfId );
         magic->mStage = stageMySpaceArtistPlaylist;
         mpMagicQueue->addMagic( magic );
      }
   }
}


void TheMagic::parseMySpaceArtistPlaylist()
{
   int pos;
   const QString songId("songId=");
   
   QStringList qsl( mBuffer.split( QRegExp("[<>]") ) );
   for( int i = 0; i < qsl.size()-1; i++ )
   {
      const QString line( qsl.at(i) );
      
      pos = line.indexOf( songId, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         mMySpaceSongId = xmlParam( qsl.at(i), pos );
         
         TheMagic *magic = new TheMagic( *this );
         magic->mURL = QString("http://musicservices.myspace.com/Modules/MusicServices/Services/MusicPlayerService.ashx?"
                               "action=getSong&songId=");
         magic->mURL.append( mMySpaceSongId );
         magic->mStage = stageMySpaceSong;
         mpMagicQueue->addMagic( magic );
      }
   }
}


void TheMagic::parseMySpaceSong()
{
   int pos;
   const QString title("title");
   const QString link("link");
   const QString artist("artist");
   const QString name("name=");
   const QString endtrack("/track");
   QString xmlArtist;
   QString xmlTitle;
   
   QStringList qsl( mBuffer.split( QRegExp("[<>]") ) );
   for( int i = 0; i < qsl.size() - 1; i++ )
   {
      const QString line( qsl.at(i) );
      
      if( line.startsWith( title, Qt::CaseInsensitive ) )
      {
         xmlTitle = qsl.at(i+1);
      }
      
      if( line.startsWith( link, Qt::CaseInsensitive ) )
      {
         mURL = qsl.at(i+1);
      }
      
      if( line.startsWith( artist, Qt::CaseInsensitive ) )
      {
         pos = line.indexOf( name, 0, Qt::CaseInsensitive );
         if( pos >= 0 )
         {
            xmlArtist = xmlParam( qsl.at(i), pos );
         }
      }
      
      pos = line.indexOf( endtrack, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         if( !(xmlArtist.isEmpty()) && !(xmlTitle.isEmpty()) && !(mURL.isEmpty()) )
         {
            TheMagic *magic = new TheMagic( *this );
            magic->mFileName = xmlArtist;
            magic->mFileName.append( QString(" - ") );
            magic->mFileName.append( xmlTitle );
            magic->mFileName.append( QString(".mp3") );
            magic->mStage = stageMySpaceMP3;
            mpMagicQueue->addMagic( magic );
         }
         xmlTitle.clear();
         xmlArtist.clear();
      }
   }
}