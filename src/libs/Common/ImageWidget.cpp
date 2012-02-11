/*
 * src/libs/Common/ImageWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "ImageWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */

#include "Trace.hpp"


ImageWidget::ImageWidget( QWidget *parent )
: QWidget( parent )
, mAspectRatioMode( Qt::KeepAspectRatio )
, mTransformationMode( Qt::SmoothTransformation )
, mpImage( 0 )
{

}

ImageWidget::~ImageWidget()
{
   if( mpImage )
   {
      delete mpImage;
   }
}


void ImageWidget::paintEvent( QPaintEvent *event )
{
TRACESTART(ImageWidget::paintEvent)
   Q_UNUSED( event );
   float ratio = 0.0;
   if( mpImage && !mpImage->isNull() )
   {
      QImage image( mpImage->scaled( size(), mAspectRatioMode, mTransformationMode ) );
      QPainter widgetPainter( this );
      int x = ( width() - image.width() ) / 2;
      int y = ( height() - image.height() ) / 2;
      widgetPainter.drawImage( x, y, image );
TRACEMSG << x << y;
      if( x == 0 )
      {
         ratio = (float)(width()) / mpImage->width();
      }
      else
      {
         ratio = (float)(height()) / mpImage->height();
      }
   }
   else
   {
      QPainter widgetPainter( this );
      widgetPainter.eraseRect( rect() );
   }
TRACEMSG << ratio;
   emit currentRatio( ratio );
}


void ImageWidget::setImage( const QImage &image )
{
TRACESTART(ImageWidget::setImage)
   if( mpImage )
   {
      delete mpImage;
   }
   mpImage = new QImage( image );
   repaint();
}


void ImageWidget::setImage( const QString &fileName )
{
   setImage( QImage( fileName ) );
}


void ImageWidget::setAspectRatioMode( Qt::AspectRatioMode mode )
{
   mAspectRatioMode = mode;
}


void ImageWidget::setTransformationMode( Qt::TransformationMode mode )
{
   mTransformationMode = mode;
}
