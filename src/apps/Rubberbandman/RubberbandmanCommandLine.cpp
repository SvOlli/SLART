/*
 * src/app/Rubberbandman/RubberbandmanCommandLine.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "RubberbandmanCommandLine.hpp"

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QCoreApplication>
#include <QTimer>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <Settings.hpp>

/* local headers */
#include "Console.hpp"
#include "DatabaseWorker.hpp"


RubberbandmanCommandLine::RubberbandmanCommandLine( const QStringList &args, QObject *parent )
: QObject( parent )
, mpDatabaseWorker( new DatabaseWorker( this ) )
, mpConsole( new Console( this ) )
, mBaseDir( Settings::value( Settings::GlobalMusicBase ) )
, mArgs( args )
{
   if( !DatabaseInterface::exists() )
   {
      mpConsole->message( tr("No database found.") );
      QTimer::singleShot( 0, this, SIGNAL(done()) );
   }
   else
   {
      connect( mpDatabaseWorker, SIGNAL(message(QString)),
               mpConsole, SLOT(message(QString)) );
      connect( mpDatabaseWorker, SIGNAL(progress(int,int)),
               mpConsole, SLOT(handleProgress(int,int)) );
      connect( mpDatabaseWorker, SIGNAL(done()),
               this, SLOT(jobDone()) );
      connect( mpDatabaseWorker, SIGNAL(done()),
               this, SLOT(nextJob()) );
   }
}


RubberbandmanCommandLine::~RubberbandmanCommandLine()
{
}


void RubberbandmanCommandLine::nextJob()
{
   if( mArgs.size() > 0 )
   {
      const QString _cleanup( "-cleanup" );
      const QString _basedir( "-basedir" );
      const QString _update( "-update" );

      QString arg( mArgs.takeFirst() );
      if( arg == _basedir )
      {
         if( mArgs.size() > 1 )
         {
            mBaseDir = mArgs.takeFirst();
         }
         QTimer::singleShot( 0, this, SLOT(nextJob()) );
      }
      else if( arg == _cleanup )
      {
         mpConsole->setTexts( tr( "entries checked" ),
                              tr( "cleaned" ) );
         QTimer::singleShot( 0, mpDatabaseWorker, SLOT(startCleanup()) );
      }
      else if( arg == _update )
      {
         if( !mBaseDir.isEmpty() )
         {
            if( !QFileInfo( mBaseDir ).isDir() )
            {
               mpConsole->message( tr( "%1 is not a directory\n" ).arg( mBaseDir ) );
            }
            else
            {
               mpDatabaseWorker->setPath( mBaseDir );
               mpConsole->setTexts( tr( "files scanned" ),
                                    tr( "updated" ) );
               QTimer::singleShot( 0, mpDatabaseWorker, SLOT(startUpdate()) );
            }
         }
      }
      else
      {
         mpConsole->message( tr("Usage:\t%1 (%2) (%3 <directory>) (%4)\n")
                                 .arg( QCoreApplication::applicationName(),
                                       _cleanup, _basedir, _update ) );
         emit done();
      }
   }
   else
   {
      emit done();
   }
}


void RubberbandmanCommandLine::jobDone()
{
   mpConsole->message( tr("Done.") );
}
