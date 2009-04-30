/**
 * FreeDB.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FreeDB.hpp"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

#include "FreeDBQuery.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


FreeDB::FreeDB( QObject *parent )
: QObject( parent )
, mpSqlDB( 0 )
, mpSearchQuery( 0 )
, mpInfoQuery( 0 )
{
   MySettings settings;
   QString dbFileName( settings.value( "DatabaseFile", "freedb.sqlite" ).toString() );
   settings.setValue( "DatabaseFile", dbFileName );
   
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
   
   if( mpSqlDB->lastError().type() != QSqlError::NoError )
   {
      QMessageBox::critical( 0, QApplication::applicationName() + QWidget::tr(": Error"),
                             QWidget::tr("Could not open FreeDB.\nPlease make sure that the SQLite driver for Qt is installed.") );
      exit(1);
   }
   
   mpSqlDB->setDatabaseName( dbFileName );
   
   if( !mpSqlDB->open() )
   {
      QMessageBox::critical( 0, QApplication::applicationName() + QWidget::tr(": Error"),
                             QWidget::tr("Could not open FreeDB.\nIs the FreeDB file readable.") );
      exit(1);
   }
   
   QSqlQuery q;
   q.exec( "SELECT * FROM sqlite_master WHERE type = 'table' AND name = 'freedb';" );
   if( !(q.next()) )
   {
      q.clear();
      q.exec("CREATE TABLE freedb("
             "category VARCHAR,id VARCHAR,track INT,title VARCHAR,playtime INT,ext VARCHAR,"
             "primary key(category,id,track));");
      q.exec("CREATE INDEX freedb_title ON freedb(title);");
   }
}


FreeDB::~FreeDB()
{
   mpSqlDB->commit();
   mpSqlDB->close();
   *mpSqlDB = QSqlDatabase();
   delete mpSqlDB;
}
