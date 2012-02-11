/*
 * src/tests/TestAppWebServer/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QSqlQueryModel>
#include <QTimer>

/* local library headers */
#include <Database.hpp>
#include <MySettings.hpp>

/* local headers */


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpModel( new QSqlQueryModel() )
, mpView( new QTableView( this ) )
, mpInfo( new QLabel( this ) )
, mTime()
{
   QTimer *refreshTimer = new QTimer( this );
   mpModel->setQuery( "select id,directory,filename,artist,title,album,"
                      "tracknr as 'Nr',year,genre,playtime as 'Time',"
                      "timesplayed as 'TP',folders,flags from slart_tracks" );

   mpView->setModel( mpModel );
   mpView->hideColumn( 0 );
   mpView->hideColumn( 1 );
   mpView->hideColumn( 2 );
   mpView->show();

   QBoxLayout *mainLayout   = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );

   mainLayout->addWidget( mpView );
   mainLayout->addWidget( mpInfo );
   setLayout( mainLayout );

   connect( mpView, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(handleClicked(QModelIndex)) );
   connect( refreshTimer, SIGNAL(timeout()),
            mpView, SLOT(resizeColumnsToContents()) );
   refreshTimer->start( 1000 );

   mTime.start();
   QTimer::singleShot( 0, this, SLOT(fillTableFull()) );
}


MainWidget::~MainWidget()
{
   if( mpDatabase )
   {
      delete mpDatabase;
      mpDatabase = 0;
   }
}


void MainWidget::fillTableFull()
{
   if( mpModel->canFetchMore() )
   {
      mpModel->fetchMore();
      QTimer::singleShot( 0, this, SLOT(fillTableFull()) );
   }
   else
   {
      mpInfo->setText( tr("Reading database done: %1s").arg( (float)mTime.elapsed()/1000 ) );
   }
}


void MainWidget::handleClicked( const QModelIndex &index )
{
   mpInfo->setText( tr("(%1,%2): %3").arg( QString::number(index.row()),
                                           QString::number(index.column()),
                                           index.data().toString()) );
}


void MainWidget::changeHeight( int fromRow, int toRow )
{
   for( int i = fromRow; i < toRow; i++ )
   {
      mpView->resizeRowToContents( i );
   }
}
