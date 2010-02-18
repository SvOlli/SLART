/**
 * src/apps/Notorious/FreeDBImport.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "FreeDBImport.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>

#include "Trace.hpp"

#include "tir.h"


FreeDBImport::FreeDBImport( QObject *parent )
: QThread( parent )
, mTarEntry()
{
}


FreeDBImport::~FreeDBImport()
{
}


void FreeDBImport::run()
{
   unsigned long int count = 0;
   int               status;
   tir_data          tarData;
   QStringList       l;
   QSqlQuery         q;
   
   status = tir_open( mFileName.toLocal8Bit().constData(), &tarData );
   if( status )
   {
      tir_close( &tarData );
      return;
   }
   
   mBreak = false;
   emit processed( count, "" );
   q.exec( "BEGIN TRANSACTION;" );
   while( !(status = tir_read( &tarData )) && !mBreak )
   {
      if( (*(tarData.filetype) == '0') && (tarData.filesize > 0) )
      {
         mTarEntry.setData( tarData.filename, tarData.data );
         l = mTarEntry.sql();
         for( int i = 0; i < l.count(); i++ )
         {
            q.exec( l.at(i) );
         }
//qDebug() << l << endl;
         if( ++count % 1000 == 0 )
         {
            q.exec( "COMMIT;");
            q.exec( "BEGIN TRANSACTION;" );
            emit processed( count, tarData.filename );
         }
      }
//      if( count >= 10000 ) mBreak = true;
   }
   q.exec( "COMMIT;" );
   emit processed( count, "" );
   
   tir_close( &tarData );
}
