/*
 * src/apps/Karmadrome/ImportExport.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ImportGroup.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTimer>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <TrackInfo.hpp>

/* local headers */

#include <Trace.hpp>

ImportGroup::ImportGroup( const QString &folder, const QString &fileName, bool cleanImport )
: QObject( 0 )
, mpDatabase( DatabaseInterface::get() )
, mQueueSize( 0 )
, mFavorite( QChar(1) )
, mUnwanted( QChar(2) )
, mGroup( folder )
{
TRACESTART(ImportGroup::ImportGroup)
   if( mGroup.startsWith( "|F", Qt::CaseInsensitive ) )
   {
      mGroup = mFavorite;
   }
   if( mGroup.startsWith( "|U", Qt::CaseInsensitive ) )
   {
      mGroup = mUnwanted;
   }

   if( cleanImport )
   {
      mpDatabase->deleteGroup( folder );
   }
   mpDatabase->insertGroup( folder );

   QFile m3uFile( fileName );
   if( m3uFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
   {
      QString fileName;
      QString fileBase( m3uFile.fileName() + "/../" );
      QFileInfo qfi;
      while( !m3uFile.atEnd() )
      {
         fileName = QString::fromLocal8Bit( m3uFile.readLine().constData() );
         if( !fileName.startsWith("#") )
         {
            if( fileName.endsWith("\n") )
            {
               fileName.chop(1);
            }
            if( !fileName.startsWith( "/" ) )
            {
               /* a bit of an ugly trick, but gets the job done better than most
                  other solutions */
               qfi.setFile( fileBase + fileName );
               fileName = qfi.absoluteFilePath();
            }
            mQueueSize++;

            mpDatabase->getTrackInfo( this, SLOT(addEntryToGroup(TrackInfo)), fileName );
         }
      }
      m3uFile.close();
   }
}


ImportGroup::~ImportGroup()
{
TRACESTART(ImportGroup::~ImportGroup)
}


void ImportGroup::addEntryToGroup( const TrackInfo &ti )
{
TRACESTART(ImportGroup::addEntryToGroup)

   TrackInfo trackInfo( ti );
   if( mGroup == mFavorite )
   {
      trackInfo.setFlag( TrackInfo::Favorite, true );
   }
   else if( mGroup == mUnwanted )
   {
      trackInfo.setFlag( TrackInfo::Unwanted, true );
   }
   else
   {
      trackInfo.setGroup( mGroup, true );
   }
   if( ti != trackInfo )
   {
      mpDatabase->updateTrackInfo( trackInfo );
   }
TRACEMSG << mQueueSize;
   if( (--mQueueSize) == 0 )
   {
      deleteLater();
   }
}
