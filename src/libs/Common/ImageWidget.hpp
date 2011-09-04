/**
 * src/libs/Common/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef IMAGEWIDGET_HPP
#define IMAGEWIDGET_HPP IMAGEWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QImage;

/* forward declaration of local classes */


class ImageWidget : public QWidget
{
   Q_OBJECT

public:
   ImageWidget( QWidget *parent );
   virtual ~ImageWidget();

protected:
   /* intercepting paint event for scaled drawing */
   void paintEvent( QPaintEvent *event );

public slots:
   /* set the image data */
   void setImage( const QImage &image );
   /* load image data from file */
   void setImage( const QString &fileName );
   /* set aspect ratio: Qt::IgnoreAspectRatio, Qt::KeepAspectRatio, Qt::KeepAspectRatioByExpanding */
   void setAspectRatioMode( Qt::AspectRatioMode mode );
   /* set transformation mode: Qt::FastTransformation, Qt::SmoothTransformation */
   void setTransformationMode( Qt::TransformationMode mode );

signals:
   void currentRatio( float radio );

private:
   ImageWidget( const ImageWidget &that );
   ImageWidget &operator=( const ImageWidget &that );

   Qt::AspectRatioMode     mAspectRatioMode;
   Qt::TransformationMode  mTransformationMode;
   QImage                  *mpImage;
};

#endif /* IMAGEWIDGET_HPP_ */
