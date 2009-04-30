/**
 * FreeDBImport.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
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
         q.exec( mTarEntry.sql() );
         if( ++count % 1000 == 0)
         {
            q.exec( "COMMIT; BEGIN TRANSACTION;" );
            emit processed( count, tarData.filename );
         }
      }
   }
   q.exec( "COMMIT;" );
   emit processed( count, "" );
   
   tir_close( &tarData );
}
