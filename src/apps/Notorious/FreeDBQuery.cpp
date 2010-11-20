/**
 * src/apps/Notorious/FreeDBQuery.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FreeDBQuery.hpp"

/* system headers */

/* Qt headers */
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>

/* local library headers */

/* local headers */
#include "Foundlist.hpp"
#include "FreeDB.hpp"


FreeDBQuery::FreeDBQuery( Foundlist *parent )
: QThread( parent )
, mpParent( parent )
{
}

#define USE_TRACE 0
#if USE_TRACE
#include <Trace.hpp>
#endif


FreeDBQuery::~FreeDBQuery()
{
}


void FreeDBQuery::run()
{
#if USE_TRACE
TRACESTART(FreeDBQuery::run);
#endif
   QStringList categories;
   if( mCategory.isNull() )
   {
      categories = FreeDB::Categories();
   }
   else
   {
      categories.clear();
      categories << mCategory;
   }

   emit running( true );
   foreach( const QString &category, categories )
   {
      QSqlQuery q( mQuery.arg( category ) );

#if USE_TRACE
      if( !q.exec() )
      {
TRACEMSG << q.lastError().databaseText();
TRACEMSG << q.lastError().driverText();
TRACEMSG << q.lastError().text();
TRACEMSG << q.lastQuery();
      }
#else
      q.exec();
#endif

      mBreak = false;
      while( q.next() )
      {
         if( mBreak )
         {
            return;
         }
         mEntries.append( FreeDBEntry( category
                                       , q.value(0).toString()
                                       , q.value(1).toInt()
                                       , q.value(2).toString()
                                       , q.value(3).toInt()
#if INCLUDE_EXT
                                       , q.value(4).toString()
#endif
                                       ) );
         //mpParent->handleNewData( mEntries.size() );
         emit newData( mEntries.size() );
      }
   }
   emit running( false );
}


void FreeDBQuery::setQuery( const QString &query, const QString &category )
{
   mCategory = category;
   mEntries.clear();
#if INCLUDE_EXT
   mQuery = QString("SELECT id, track, title, playtime, ext FROM freedb_%1 WHERE ");
#else
   mQuery = QString("SELECT id, track, title, playtime FROM freedb_%1 WHERE ");
#endif
   //mQuery = QString("SELECT * FROM freedb_%1 WHERE ");
   mQuery.append( query );
}
