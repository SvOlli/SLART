/**
 * src/apps/Notorious/FreeDB.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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

#define USE_SQLITE 1

FreeDB::FreeDB( QObject *parent )
: QObject( parent )
, mpSqlDB( 0 )
, mpSearchQuery( 0 )
, mpInfoQuery( 0 )
{
   MySettings settings;
   QString dbFileName( settings.value( "DatabaseFile", "freedb.sqlite" ).toString() );
   settings.setValue( "DatabaseFile", dbFileName );

#if USE_SQLITE
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) );
#else
   mpSqlDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );
#endif
   
   if( mpSqlDB->lastError().type() != QSqlError::NoError )
   {
      QMessageBox::critical( 0, QApplication::applicationName() + QWidget::tr(": Error"),
                             QWidget::tr("Could not open FreeDB.\nPlease make sure that the SQLite driver for Qt is installed.") );
      exit(1);
   }
   
#if USE_SQLITE
   mpSqlDB->setDatabaseName( dbFileName );
#else
   mpSqlDB->setDatabaseName( "freedb" );
   mpSqlDB->setUserName( "svolli" );
   mpSqlDB->setPassword( "svolli" );
#endif
   
   if( !mpSqlDB->open() )
   {
      QMessageBox::critical( 0, QApplication::applicationName() + QWidget::tr(": Error"),
                             QWidget::tr("Could not open FreeDB.\nIs the FreeDB file readable.") );
      exit(1);
   }
   
   QSqlQuery q;
#if USE_SQLITE
   q.exec( "SELECT * FROM sqlite_master WHERE type = 'table' AND name = 'freedb';" );
   if( !(q.next()) )
   {
      q.clear();
      q.exec("CREATE TABLE freedb("
             "category VARCHAR,id VARCHAR,track INT,title VARCHAR,playtime INT,ext VARCHAR,"
             "PRIMARY KEY(category,id,track));");
      q.exec("CREATE INDEX freedb_title ON freedb(title);");
   }
#endif
}


FreeDB::~FreeDB()
{
   mpSqlDB->commit();
   mpSqlDB->close();
   *mpSqlDB = QSqlDatabase();
   delete mpSqlDB;
}
