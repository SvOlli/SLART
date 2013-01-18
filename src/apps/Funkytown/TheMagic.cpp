/*
 * src/apps/Funkytown/TheMagic.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "TheMagic.hpp"

/* system headers */

/* Qt headers */
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>

/* local library headers */
#include <Satellite.hpp>
#include <Settings.hpp>

/* local headers */
#include "MagicQueue.hpp"

#define USE_TRACE 0

#define RTMP_TO_STDOUT 0

#if USE_TRACE
#include <Trace.hpp>
#endif


TheMagic::TheMagic( MagicQueue *magicQueue )
: QObject()
, mURL()
, mSelected( false )
, mStage( stageFresh )
, mpMagicQueue( magicQueue )
, mpSatellite( Satellite::get() )
, mDownloadToFile( false )
, mSuccess( true )
, mContentType()
, mReferer()
, mMessage()
, mFileName()
, mBuffer()
, mpFile( 0 )
, mpBuffer( 0 )
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
}


/*
 * Object that should not be copied from one object to another:
 * - mpFile
 * - mpBuffer
 */

TheMagic::TheMagic( const TheMagic &that )
: QObject()
, mURL              ( that.mURL )
, mSelected         ( that.mSelected )
, mStage            ( that.mStage )
, mpMagicQueue      ( that.mpMagicQueue )
, mpSatellite       ( that.mpSatellite )
, mDownloadToFile   ( that.mDownloadToFile )
, mSuccess          ( true )
, mContentType      ( that.mContentType )
, mReferer          ( that.mReferer )
, mMessage          ( /*other.mMessage*/ )
, mFileName         ( that.mFileName )
, mBuffer           ( that.mBuffer )
, mpFile            ( 0 )
, mpBuffer          ( 0 )
, mMySpaceArtId     ( that.mMySpaceArtId )
, mMySpaceProfId    ( that.mMySpaceProfId )
, mMySpacePlaylistId( that.mMySpacePlaylistId )
, mMySpaceSongId    ( that.mMySpaceSongId )
{
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


QString TheMagic::referer()
{
   return mReferer;
}


void TheMagic::fail()
{
   mSuccess = false;
}


void TheMagic::setContentType( const QString &contentType )
{
   mContentType = contentType;
}


QIODevice *TheMagic::ioDevice()
{
   if( mDownloadToFile )
   {
      return reinterpret_cast<QIODevice*>(mpFile);
   }
   else
   {
      return reinterpret_cast<QIODevice*>(mpBuffer);
   }
}


void TheMagic::downloadToFile()
{
   sanitizeFileName();
   mpFile = new QFile( mFileName );

   if( !Settings::value( Settings::FunkytownOverwrite ) )
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
      if( mContentType.contains( "UTF-8", Qt::CaseInsensitive ) || mContentType.contains( "UTF8", Qt::CaseInsensitive ) )
      {
         mBuffer = QString::fromUtf8( mpBuffer->buffer().constData() );
      }
      else if( mContentType.contains( "8859-1", Qt::CaseInsensitive ) )
      {
         mBuffer = QString::fromLatin1( mpBuffer->buffer().constData() );
      }
      else
      {
         mBuffer = QString::fromLocal8Bit( mpBuffer->buffer().constData() );
      }
      mpBuffer->close();
      delete mpBuffer;
      mpBuffer = 0;
   }
}



void TheMagic::sanitizeFileName()
{
   // replace all html-escaped characters
   mFileName.replace( QRegExp( "&[^&]*;" ), "_" );
   // remove illegal characters
   mFileName.remove( QRegExp("[\\\\:?]") );
   // replace other illegal characters
   mFileName.replace( QRegExp("[<>;\"|/\\*]"), "_" );
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
#if USE_TRACE
TRACESTART(DownloadHandler::preDownload)
#endif
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
#if USE_TRACE
TRACESTART(DownloadHandler::postDownload)
#endif
   mSuccess &= success;
   downloadClose();

   if( mSuccess )
   {
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
}


void TheMagic::parseURL()
{
#if USE_TRACE
TRACESTART(TheMagic::parseURL)
TRACEMSG << mURL;
#endif
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
      if( Settings::value( Settings::FunkytownTollKeep ) )
      {
         qulonglong total = Settings::value( Settings::FunkytownBytes );
         total += fileinfo.size();
         Settings::setValue( Settings::FunkytownBytes, total );

         unsigned int count = Settings::value( Settings::FunkytownFiles ) + 1;
         Settings::setValue( Settings::FunkytownFiles, count );
      }

      if( mpSatellite )
      {
         mpSatellite->send( QByteArray("f0v\n") +
                            QDir::current().absoluteFilePath( mFileName ).toUtf8() );
      }
   }
}


void TheMagic::parseGenericHTML()
{
#if USE_TRACE
TRACESTART(TheMagic::parseGenericHTML)
#endif
   int pos;
   QString param("http://www.youtube.com/v/");
   QString embed;
   QString line;
   QRegExp href( "href=.*mp3", Qt::CaseInsensitive );

   QStringList qsl( mBuffer.split( QChar('\n') ) );
   foreach( QString line, qsl )
   {
      pos = line.indexOf( param, 0, Qt::CaseInsensitive );
      if( pos > 0 )
      {
         QString embed( line );
         embed.replace( QRegExp(".*\"http://"), "http://" );
         embed.remove( QRegExp("&.*") );
         embed.replace( "/v/", "/watch?v=" );

         TheMagic *magic = new TheMagic( *this );
         magic->mURL = embed;
         magic->mStage = stageYouTubeHTML;
         magic->mReferer = mURL;

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
         magic->mURL = mp3ref;
         magic->mStage = stageGenericFile;
         magic->mReferer = mURL;

         mpMagicQueue->addMagic( magic );
      }
   }
}


void TheMagic::parseYouTubeHTML()
{
#if USE_TRACE
TRACESTART(TheMagic::parseYouTubeHTML)
#endif
   int pos;
   QString line;
   QString youTube("fullscreenUrl");
   QString t;
   QString title;
   QString videoID;

   QStringList qsl( mBuffer.split( QChar('\n') ) );
   foreach( line, qsl )
   {
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
#if USE_TRACE
TRACEMSG << magic->mURL << magic->mFileName;
#endif
      magic->mStage = stageGenericFile;
      magic->mReferer = mURL;
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
   foreach( QString line, qsl )
   {

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
      magic->mReferer = mURL;
      mpMagicQueue->addMagic( magic );

      if( !mMySpaceArtId.isEmpty() )
      {
         TheMagic *magic = new TheMagic( *this );
         magic->mURL = QString("http://musicservices.myspace.com/Modules/MusicServices/Services/MusicPlayerService.ashx?"
                               "action=getPlaylists&friendId=");
         magic->mURL.append( mMySpaceProfId );
         magic->mStage = stageMySpacePlaylists;
         magic->mReferer = mURL;
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
      }
      if( dataList.at(i) == QString("song") )
      {
         xmlTitle = "";
         xmlUrl   = "";
      }
      if( dataList.at(i).startsWith("title=") )
      {
         xmlTitle = dataList.at(i);
         xmlTitle.replace( QRegExp( ".*\"(.*)\".*" ), "\\1" );
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
      if( Settings::value( Settings::FunkytownTollKeep ) )
      {
         qulonglong total = Settings::value( Settings::FunkytownBytes );
         total += fileinfo.size();
         Settings::setValue( Settings::FunkytownBytes, total );

         unsigned int count = Settings::value( Settings::FunkytownFiles ) + 1;
         Settings::setValue( Settings::FunkytownFiles, count );
      }

      if( mpSatellite )
      {
         mpSatellite->send( QByteArray("f0d\n") +
                            QDir::current().absoluteFilePath( mFileName ).toUtf8() );
         if( mSelected )
         {
            mpSatellite->send( QByteArray("P0Q\n") +
                               QDir::current().absoluteFilePath( mFileName ).toUtf8() );
         }
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
   foreach( QString line, qsl )
   {
      pos = line.indexOf( playlistId, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         mMySpacePlaylistId = xmlParam( line, pos );

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
   const QString large("large");
   const QString name("name=");
   const QString rtmp("rtmp:");
   const QString endtrack("/track");
   QString xmlArtist;
   QString xmlTitle;
   QString xmlCover;

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

      if( line.startsWith( rtmp, Qt::CaseInsensitive ) )
      {
#if RTMP_TO_STDOUT
qDebug() << line;
#endif
      }

      if( line.startsWith( large, Qt::CaseInsensitive ) )
      {
         xmlCover = qsl.at(i+1);
      }

      pos = line.indexOf( endtrack, 0, Qt::CaseInsensitive );
      if( pos >= 0 )
      {
         if( !(xmlArtist.isEmpty()) && !(xmlTitle.isEmpty()) && !(mURL.isEmpty()) )
         {
            QString fileName( xmlArtist );
            fileName.append( QString(" - ") );
            fileName.append( xmlTitle );
            mURL.replace("std_","full_");
            mURL.replace("&amp;","&");

            TheMagic *magic = new TheMagic( *this );
            magic->mFileName = fileName;
            magic->mFileName.append( QString(".mp3") );
            magic->mStage = stageMySpaceMP3;
            mpMagicQueue->addMagic( magic );

            if( !(xmlCover.isEmpty()) && Settings::value( Settings::FunkytownCoverArt ) )
            {
               TheMagic *magic = new TheMagic( *this );
               magic->mURL = xmlCover.trimmed();
               magic->mSelected = false;
               magic->mFileName = fileName;
               magic->mFileName.append( QString(".jpg") );
               magic->mStage = stageGenericFile;
               mpMagicQueue->addMagic( magic );
            }
         }
         xmlTitle.clear();
         xmlArtist.clear();
         xmlCover.clear();
      }
   }
}
