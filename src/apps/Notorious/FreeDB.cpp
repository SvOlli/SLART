/**
 * src/apps/Notorious/FreeDB.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FreeDB.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

/* local library headers */
#include "MySettings.hpp"

/* local headers */
#include "FreeDBQuery.hpp"


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
   QStringList categories( Categories() );
   for( int i = 0; i < categories.size(); i++ )
   {
      q.exec( QString("SELECT * FROM sqlite_master WHERE type = 'table' AND name = 'freedb_%1';").arg(categories.at(i)) );
      if( !(q.next()) )
      {
         q.clear();
         q.exec(QString("CREATE TABLE freedb_%1("
#if INCLUDE_EXT
                "id VARCHAR,track INT,title VARCHAR,playtime INT,ext VARCHAR,"
#else
                "id VARCHAR,track INT,title VARCHAR,playtime INT,"
#endif
                "PRIMARY KEY(id,track));").arg(categories.at(i)));
      }
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


QStringList FreeDB::Categories()
{
   return QString("blues classical country data folk jazz misc newage reggae rock soundtrack").split(' ');
}
