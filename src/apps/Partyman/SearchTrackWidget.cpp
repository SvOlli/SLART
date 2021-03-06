/*
 * src/apps/Partyman/SearchTrackWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SearchTrackWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QLabel>

/* local library headers */
#include <Database.hpp>
#include <Settings.hpp>

/* local headers */
#include "PartymanMainWindow.hpp"
#include "PlaylistContentWidget.hpp"
#include "SearchLineEdit.hpp"


SearchTrackWidget::SearchTrackWidget( Database *database, QWidget *parent )
: QWidget( parent )
, mpDatabase( database )
, mpParent( parent )
, mpResults( new PlaylistContentWidget( database, false, this ) )
, mpInput( new SearchLineEdit( this ) )
, mpFound( new QLabel( this ) )
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *lineLayout = new QHBoxLayout;
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
   lineLayout->setContentsMargins( 0, 0, 0, 0 );
   lineLayout->addWidget( mpInput );
   lineLayout->addWidget( mpFound );
   mpFound->setHidden( true );

   mainLayout->setSpacing( 2 );
   mainLayout->addWidget( mpResults );
   mainLayout->addLayout( lineLayout );
   setLayout( mainLayout );
   mpInput->setMouseTracking( true );

   connect( mpInput, SIGNAL(returnPressed()),
            this, SLOT(search()) );
   connect( mpResults, SIGNAL(context(QModelIndex,int)),
            this, SLOT(selectedEntries(QModelIndex,int)) );
   connect( mpFound, SIGNAL(customContextMenuRequested(QPoint)),
            mpInput, SLOT(clear()) );
   connect( mpResults, SIGNAL(dataRemoved()),
            this, SLOT(updateCounter()) );

   mpInput->setText( Settings::value( Settings::PartymanSearch ) );
   Settings::setValue( Settings::PartymanSearch, QString() );
   mpInput->selectAll();
   mpInput->setFocus();
}


SearchTrackWidget::~SearchTrackWidget()
{
   Settings::setValue( Settings::PartymanSearch, mpInput->text() );
}


void SearchTrackWidget::search()
{
   mpInput->selectAll();
   mpResults->clear();
   if( mpInput->text().size() > 0 )
   {
      QStringList found;
      int count = mpDatabase->getPathNameList( &found, mpInput->text() );
      mpFound->setText( tr("%1 Found").arg(count) );
      mpResults->addItems( found );
      mpFound->setHidden( false );
      mpResults->setFocus();
   }
   else
   {
      mpFound->setHidden( true );
   }
}


void SearchTrackWidget::updateCounter()
{
   mpFound->setText( tr("%1 Left").arg(mpResults->count()) );
   if( !mpResults->count() )
   {
      mpFound->setHidden( true );
   }
}


void SearchTrackWidget::selectedEntries( const QModelIndex &/*index*/, int key )
{
   QStringList entries;
   switch( key )
   {
      case 0: /* right mouse button */
      case Qt::Key_Return:
      case Qt::Key_Enter:
      case Qt::Key_Delete:
         mpResults->removeSelectedItems( &entries );
         updateCounter();
         if( key != Qt::Key_Delete )
         {
            emit requestAdd( entries );
         }
         break;
      case Qt::Key_Escape:
         mpResults->clear();
         mpFound->setHidden( true );
         mpInput->setFocus();
         mpInput->selectAll();
         break;
      default:
         break;
   }
}


void SearchTrackWidget::setFocus()
{
   mpInput->setFocus();
   mpInput->selectAll();
}
