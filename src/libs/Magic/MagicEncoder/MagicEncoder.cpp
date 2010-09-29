/**
 * src/apps/Magic/MagicEncoder/MagicEncoder.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MagicEncoder.hpp"

/* system headers */

/* Qt headers */
#include <QDir>
#include <QFileDialog>

/* local library headers */
#include "../../libs/Common/MagicEncoderProxy.hpp"
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <ScrollLine.hpp>

/* local headers */


#include <Trace.hpp>


MagicEncoder::MagicEncoder( const QString &encoderName )
: QThread( 0 )
, mpProxy( 0 )
, mUseEncoder( false )
, mEnqueue( false )
, mDirOverride( false )
, mDirectory()
, mName( encoderName )
, mFile()
, mTagList()
{
   QThread::moveToThread( this );
}


MagicEncoder::~MagicEncoder()
{
}


QThread *MagicEncoder::workerThread()
{
   return this;
}


void MagicEncoder::setup( MagicEncoderProxy *proxy, const QString &msgHeader )
{
   mpProxy    = proxy;
   mMsgHeader = msgHeader;
}

QString MagicEncoder::pluginFileName()
{
   return mPluginFileName;
}


QString MagicEncoder::name()
{
   return mName;
}


void MagicEncoder::run()
{
   /* when started just run event loop to handle messages */
   exec();
}


bool MagicEncoder::initialize( const QString &fileName, const char *extension )
{
   QString directory = mDirOverride ? mDirectory : MySettings().VALUE_DIRECTORY;

   QFileInfo qfi( directory + QDir::separator() +
                  fileName + extension );
   QDir dir( qfi.absoluteDir() );
   
   if( !dir.exists() )
   {
      if( !dir.mkpath( dir.absolutePath() ) )
      {
         return false;
      }
   }

   mFileName = qfi.absoluteFilePath();
   mFile.setFileName( mFileName );
   return mFile.open( QIODevice::WriteOnly );
}


bool MagicEncoder::finalize( bool enqueue, bool cancel )
{
   mFile.close();
   
   if( !cancel )
   {
      if( mpProxy )
      {
         QString msg( "%1\n%2" );
         mpProxy->satelliteSend( msg.arg( mMsgHeader, mFileName ).toUtf8() );
         if( enqueue && mEnqueue )
         {
            mpProxy->satelliteSend( msg.arg( "P0Q", mFileName ).toUtf8() );
         }
      }
   }
   return !cancel;
}


bool MagicEncoder::writeChunk( const char* buffer, qint64 size )
{
#if 0
TRACESTART(Encoder::writeChunk);
TRACEMSG << mName << hex << QThread::currentThreadId();
#endif
   while( size > 0 )
   {
      qint64 written = mFile.write( buffer, size );
      if( written < 0 )
      {
         return false;
      }
      size   -= written;
      buffer += written;
   }
   return true;
}


void MagicEncoder::setTags( const TagList &tagList )
{
   mTagList = tagList;
}


bool MagicEncoder::useEncoder()
{
   return mUseEncoder;
}


void MagicEncoder::setEnqueue( bool activate )
{
   mEnqueue = activate;
}


void MagicEncoder::setDirectory( ScrollLine *dirOverride )
{
   QFileDialog fileDialog( dirOverride );

   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( dirOverride->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      dirOverride->setText( result );
   }
}
