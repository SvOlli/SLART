/*
 * src/libs/Common/DropImageWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "DropImageWidget.hpp"

/* system headers */

/* Qt headers */
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>

/* local library headers */

/* local headers */



DropImageWidget::DropImageWidget( QWidget *parent )
: ImageWidget( parent )
{
   setAcceptDrops( true );
}


DropImageWidget::~DropImageWidget()
{
}


void DropImageWidget::dragEnterEvent( QDragEnterEvent *event )
{
   const QMimeData *mimeData = event->mimeData();

   if( mimeData->hasUrls() )
   {
      event->acceptProposedAction();
   }
}


void DropImageWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();

   if( mimeData->hasUrls() )
   {
      QList<QUrl> urls( mimeData->urls() );
      if( urls.size() > 0 )
      {
         emit droppedUrl( urls.at(0) );
         event->acceptProposedAction();
      }
   }
}
