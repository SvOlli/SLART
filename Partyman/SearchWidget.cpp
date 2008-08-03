/**
 * SearchWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SearchWidget.hpp"
#include "PlaylistWidget.hpp"
#include "PlaylistContentWidget.hpp"
#include "Database.hpp"
#include "MySettings.hpp"

#include <QtGui>


class SearchLineEdit : public QLineEdit
{
public:
   SearchLineEdit( SearchWidget *parent );
protected:
   virtual void keyPressEvent( QKeyEvent *event );
private:
   SearchWidget *mpSearchWidget;
};


SearchLineEdit::SearchLineEdit( SearchWidget *parent )
: QLineEdit( parent )
, mpSearchWidget( parent )
{
}


void SearchLineEdit::keyPressEvent( QKeyEvent *event )
{
   if( event->key() == Qt::Key_Escape )
   {
      if( text().count() > 0 )
      {
         clear();
      }
      else
      {
         mpSearchWidget->selectedEntries( QModelIndex(), Qt::Key_Escape );
      }
   }
   QLineEdit::keyPressEvent( event );
}


SearchWidget::SearchWidget( Database *database, PlaylistWidget *parent )
: QWidget( parent )
, mpDatabase( database )
, mpPlaylist( parent )
, mpResults( new PlaylistContentWidget( database, false, this ) )
, mpInput( new SearchLineEdit( this ) )
, mpFound( new QLabel( this ) )
{
   MySettings settings;
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *lineLayout = new QHBoxLayout;
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 1 );
   lineLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
   lineLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
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
   
   mpInput->setText( settings.value( "Search", QString()).toString() );
   settings.remove( "Search" );
   mpInput->selectAll();
   mpInput->setFocus();
}


SearchWidget::~SearchWidget()
{
   MySettings settings;
   settings.setValue( "Search", mpInput->text() );
}


void SearchWidget::search()
{
   mpInput->selectAll();
   mpResults->clear();
   if( mpInput->text().size() > 0 )
   {
      TrackInfoList til;
      int count;
      
      count = mpDatabase->getTrackInfoList( &til, mpInput->text() );
      mpFound->setText( QString::number(count) + 
                        " Found" );
      mpResults->addItems( til );
      mpFound->setHidden( false );
      mpResults->setFocus();
   }
   else
   {
      mpFound->setHidden( true );
   }
}


void SearchWidget::selectedEntries( const QModelIndex &/*index*/, int key )
{
   QStringList entries;
   switch( key )
   {
      case 0: /* right mouse button */
      case Qt::Key_Delete:
      case Qt::Key_Return:
      case Qt::Key_Enter:
         mpResults->removeSelectedItems( &entries );
         mpFound->setText( QString::number(mpResults->count()) + 
                           " Left" );
         if( !mpResults->count() )
         {
            mpFound->setHidden( true );
         }
         if( key == Qt::Key_Delete ) break;
         mpPlaylist->addEntries( entries );
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


QLineEdit *SearchWidget::getLineEdit()
{
   return mpInput;
}


void SearchWidget::setFocus()
{
   mpInput->setFocus();
   mpInput->selectAll();
}
