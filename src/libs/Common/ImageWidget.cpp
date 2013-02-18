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
#include <QImage>
#include <QMouseEvent>
#include <QPainter>

/* local library headers */

/* local headers */



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
   Q_UNUSED( event );
   float ratio = 0.0;
   if( mpImage && !mpImage->isNull() )
   {
      QImage image( mpImage->scaled( size(), mAspectRatioMode, mTransformationMode ) );
      QPainter widgetPainter( this );
      int x = ( width() - image.width() ) / 2;
      int y = ( height() - image.height() ) / 2;
      widgetPainter.drawImage( x, y, image );
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
   emit currentRatio( ratio );
}


void ImageWidget::mousePressEvent( QMouseEvent *event )
{
   if( (event->button() != Qt::LeftButton) || !mpImage )
   {
      return;
   }
   emit clicked( event->pos() );
   event->accept();
}


void ImageWidget::setImage( const QImage &image )
{
   if( mpImage )
   {
      delete mpImage;
   }
   mpImage = new QImage( image );
   repaint();
}


void ImageWidget::setImage( const QByteArray &data )
{
   setImage( QImage::fromData( data ) );
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


QSize ImageWidget::minimumSizeHint() const
{
   QSize size( 20, 20 );
   if( minimumSize().width() > size.width() )
   {
      size.setWidth( minimumSize().width() );
   }
   if( minimumSize().height() > size.height() )
   {
      size.setHeight( minimumSize().height() );
   }
   return size;
}
