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


QImage ImageWidget::imageData() const
{
   return *mpImage;
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
#if 0
   QMimeData *mimeData = new QMimeData;
   if( mDragUsingTempFile )
   {
      if( mpTempFile )
      {
         delete mpTempFile;
      }
      QString tempFileName( QDir::tempPath() );
      if( mDragAsPng )
      {
         tempFileName.append( "/XXXXXX.PNG" );
      }
      else
      {
         tempFileName.append( "/XXXXXX.BMP" );
      }
      mpTempFile = new QTemporaryFile( tempFileName, this );
      mpTempFile->open();
      mpImage->save( mpTempFile, QFileInfo(tempFileName).suffix().toAscii() );
      mpTempFile->close();
      mimeData->setData( "text/uri-list", QUrl::fromLocalFile(mpTempFile->fileName()).toEncoded() );
   }
   else
   {
      QByteArray itemData;
      QBuffer buffer( &itemData );
      buffer.open( QIODevice::WriteOnly );
      if( mDragAsPng )
      {
         mpImage->save( &buffer, "PNG" );
         mimeData->setData( "image/png", itemData );
      }
      else
      {
         mpImage->save( &buffer, "BMP" );
         mimeData->setData( "image/bmp", itemData );
      }
      buffer.close();
   }

   QDrag *drag = new QDrag( this );
   drag->setMimeData( mimeData );
   //drag->setHotSpot( event->pos() /*- rect().topLeft()*/ );
   int x = 0;
   int y = 0;
   if( mpImage->width() > mpImage->height() )
   {
      x = mDragThumbSize;
      y = ( x * mpImage->height() ) / mpImage->width();
   }
   else
   {
      y = mDragThumbSize;
      x = ( y * mpImage->width() ) / mpImage->height();
   }
   drag->setPixmap( QPixmap::fromImage( mpImage->scaled( x, y ) ) );
   drag->exec( Qt::CopyAction );
#endif
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
