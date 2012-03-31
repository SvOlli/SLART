/*
 * src/apps/Notorious/Foundlist.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "Foundlist.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */


Foundlist::Foundlist( QThread::Priority priority, QWidget *parent )
: QTableWidget( parent )
, mQuery( this )
{
   mQuery.setPriority( priority );
   setSelectionBehavior( QAbstractItemView::SelectRows );
   connect( &mQuery, SIGNAL(newData(int)),
            this, SLOT(handleNewData(int)) );
   connect( &mQuery, SIGNAL(running(bool)),
            this, SIGNAL(queryRunning(bool)) );
   connect( this, SIGNAL(itemClicked(QTableWidgetItem*) ),
            this, SLOT(handleClicked(QTableWidgetItem*)) );
}


Foundlist::~Foundlist()
{
   if( !mQuery.isFinished() )
   {
      mQuery.stop();
      mQuery.wait();
   }
}


void Foundlist::handleNewData( int i )
{
   if( (i == 0) || (mQuery.mEntries.size() == 0) )
   {
      return;
   }

   setRowCount( i );
   --i;

   int playtime = mQuery.mEntries.at(i).playtime / 75;
   QString displaytime( QString::number( playtime / 60 ) );
   displaytime.append(":");
   if( (playtime % 60) < 10 )
   {
      displaytime.append("0");
   }
   displaytime.append( QString::number( playtime % 60 ) );

   QTableWidgetItem *categoryItem =
      new QTableWidgetItem( mQuery.mEntries.at(i).category );
   categoryItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
   setItem( i, 0, categoryItem );

   QTableWidgetItem *idItem =
      new QTableWidgetItem( mQuery.mEntries.at(i).id );
   idItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
   setItem( i, 1, idItem );

   QTableWidgetItem *trackItem =
      new QTableWidgetItem( QString::number( mQuery.mEntries.at(i).track ) );
   trackItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
   setItem( i, 2, trackItem );

   QTableWidgetItem *titleItem =
      new QTableWidgetItem( mQuery.mEntries.at(i).title );
   titleItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
   setItem( i, 3, titleItem );

   QTableWidgetItem *playtimeItem =
      new QTableWidgetItem( displaytime );
   playtimeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
   setItem( i, 4, playtimeItem );

#if INCLUDE_EXT
   if( mQuery.mEntries.count() > 5 )
   {
      QTableWidgetItem *extItem =
         new QTableWidgetItem( mQuery.mEntries.at(i).ext );
      categoryItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
      setItem( i, 5, extItem );
   }
#endif

   resizeColumnsToContents();
}


void Foundlist::runQuery( const QString &query, const QString &category )
{
   QStringList labels;
   labels
      << tr("Category")
      << tr("ID")
      << tr("Track")
      << tr("Title")
      << tr("Playtime")
#if INCLUDE_EXT
      << tr("Ext")
#endif
      ;

   if( !mQuery.isFinished() )
   {
      mQuery.stop();
      mQuery.wait();
   }

   clear();
#if INCLUDE_EXT
   setColumnCount( 6 );
#else
   setColumnCount( 5 );
#endif
   setRowCount( 0 );
   setHorizontalHeaderLabels( labels );

   mQuery.setQuery( query, category );
   mQuery.start();
}


void Foundlist::handleClicked( QTableWidgetItem *clickedItem )
{
   resizeColumnsToContents();
   QApplication::clipboard()->setText( item( clickedItem->row(), 3 )->text(), QClipboard::Clipboard );
   emit clicked( item( clickedItem->row(), 0 )->text(), item( clickedItem->row(), 1 )->text() );
}
