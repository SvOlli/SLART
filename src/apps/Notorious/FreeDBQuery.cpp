/**
 * src/apps/Notorious/FreeDBQuery.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "FreeDBQuery.hpp"
#include "Foundlist.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <qdebug.h>

#include "Trace.hpp"


FreeDBQuery::FreeDBQuery( Foundlist *parent )
: QThread( parent )
, mpParent( parent )
{
}


FreeDBQuery::~FreeDBQuery()
{
}


void FreeDBQuery::run()
{
#if 0
TRACESTART(FreeDBQuery::run)
#endif
   QSqlQuery q( mQuery );
   
#if 0
   if( !q.exec() )
   {
TRACEMSG << q.lastError().databaseText();
TRACEMSG << q.lastError().driverText();
TRACEMSG << q.lastError().text();
TRACEMSG << q.lastQuery();
   }
#endif
   
   mBreak = false;
   emit running( true );
   while( q.next() )
   {
      if( mBreak )
      {
         return;
      }
      mEntries.append( FreeDBEntry( q.value(0).toString(), 
                                    q.value(1).toString(),
                                    q.value(2).toInt(),
                                    q.value(3).toString(),
                                    q.value(4).toInt(),
                                    q.value(5).toString() ) );
      //mpParent->handleNewData( mEntries.size() );
      emit newData( mEntries.size() );
   }
   emit running( false );
}


void FreeDBQuery::setQuery( const QString &query )
{
   mEntries.clear();
   mQuery = QString("SELECT category, id, track, title, playtime, ext FROM freedb WHERE ");
   //mQuery = QString("SELECT * FROM freedb WHERE ");
   mQuery.append( query );
}
