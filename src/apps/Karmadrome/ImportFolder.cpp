/**
 * src/apps/Karmadrome/ImportExport.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ImportFolder.hpp"

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


ImportFolder::ImportFolder( const QString &folder, const QString &fileName, bool cleanImport )
: QObject( 0 )
, mpDatabase( DatabaseInterface::get() )
, mFavorite( QChar(1) )
, mUnwanted( QChar(2) )
, mFolder( folder )
{
   if( mFolder.startsWith( "|F", Qt::CaseInsensitive ) )
   {
      mFolder = mFavorite;
   }
   if( mFolder.startsWith( "|U", Qt::CaseInsensitive ) )
   {
      mFolder = mUnwanted;
   }

   mpDatabase->insertFolder( folder );
   if( cleanImport )
   {
      mpDatabase->getFolder( this, "removeEntryFromFolder", folder );
   }

   QFile m3uFile( fileName );
   if( m3uFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
   {
      QString fileName;
      QString fileBase( m3uFile.fileName() + "/../" );
      QFileInfo qfi;
      while( !m3uFile.atEnd() )
      {
         fileName = QString::fromLocal8Bit( m3uFile.readLine() );
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
            mpDatabase->getTrackInfo( this, "addEntryToFolder", fileName );
         }
      }
      m3uFile.close();
   }
   QTimer::singleShot( 0, this, SLOT(deleteLater()) );
}


ImportFolder::~ImportFolder()
{
}


void ImportFolder::addEntryToFolder( const TrackInfo &ti )
{
   set( ti, true );
}


void ImportFolder::removeEntryFromFolder( const TrackInfo &ti )
{
   set( ti, false );
}


void ImportFolder::set( const TrackInfo &ti, bool set )
{
   TrackInfo trackInfo( ti );
   if( mFolder == mFavorite )
   {
      trackInfo.setFlag( TrackInfo::Favorite, set );
   }
   else if( mFolder == mUnwanted )
   {
      trackInfo.setFlag( TrackInfo::Unwanted, set );
   }
   else
   {
      trackInfo.setFolder( mFolder, set );
   }
   if( ti != trackInfo )
   {
      mpDatabase->updateTrackInfo( trackInfo );
   }
}


void ImportFolder::removeEntriesFromFolder( const QStringList &entries )
{
   foreach( const QString &entry, entries )
   {
      mpDatabase->getTrackInfo( this, QString("removeEntryFromFolder"), entry );
   }
}
