/**
 * src/libs/Common/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
   /*  */
   void paintEvent( QPaintEvent *event );

public slots:
   /*  */
   void setImage( const QImage &image );
   /*  */
   void setImage( const QString &fileName );
   /* set aspect ratio: Qt::IgnoreAspectRatio, Qt::KeepAspectRatio, Qt::KeepAspectRatioByExpanding */
   void setAspectRatioMode( Qt::AspectRatioMode mode );
   /* set transformation mode: Qt::FastTransformation, Qt::SmoothTransformation */
   void setTransformationMode( Qt::TransformationMode mode );

signals:
   void currentRatio( float radio );

private:
   Q_DISABLE_COPY( ImageWidget )

   Qt::AspectRatioMode     mAspectRatioMode;
   Qt::TransformationMode  mTransformationMode;
   QImage                  *mpImage;
};

#endif /* IMAGEWIDGET_HPP_ */
