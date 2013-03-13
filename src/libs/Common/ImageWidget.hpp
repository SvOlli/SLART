/*
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
#include <QImage>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QImage;
class QMouseEvent;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief widget containing an auto-scaled image

 The image can be provided either by file name, raw data in a QByteArray or a
 QImage. It will be scaled to fit widget size automatically.

*/
class ImageWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   ImageWidget( QWidget *parent );

   /*!
    \brief destructor

   */
   virtual ~ImageWidget();

   /*!
    \brief overload to send a minimum size so image won't disappear on autosizing layouts

    \return QSize
   */
   virtual QSize minimumSizeHint() const;

protected:
   /*!
    \brief intercepting paint event for scaled drawing

    \param event
   */
   void paintEvent( QPaintEvent *event );

   /*!
    \brief intercepting mouse event for emitting click

    \param event
   */
   void mousePressEvent( QMouseEvent *event );

public slots:
   /*!
    \brief set the image data

    \param image
   */
   void setImage( const QImage &image );
   /*!
    \brief set the image data

    \param data
   */
   void setImage( const QByteArray &data );
   /*!
    \brief load image data from file

    \param fileName
   */
   void setImage( const QString &fileName );
   /*!
    \brief set aspect ratio: Qt::IgnoreAspectRatio, Qt::KeepAspectRatio, Qt::KeepAspectRatioByExpanding

    \param mode
   */
   void setAspectRatioMode( Qt::AspectRatioMode mode );
   /*!
    \brief set transformation mode: Qt::FastTransformation, Qt::SmoothTransformation

    \param mode
   */
   void setTransformationMode( Qt::TransformationMode mode );
   /*!
    \brief get image data

    \return QImage
   */
   QImage imageData() const;

signals:
   /*!
    \brief emit radio of image to stored graphics data

    \param radio
   */
   void currentRatio( float radio );

   /*!
    \brief click on image

    \param pos
   */
   void clicked( const QPoint &pos );

private:
   Q_DISABLE_COPY( ImageWidget )

   Qt::AspectRatioMode     mAspectRatioMode; /*!< \brief \todo TODO */
   Qt::TransformationMode  mTransformationMode; /*!< \brief \todo TODO */
   QImage                  *mpImage; /*!< \brief \todo TODO */
};

/*! @} */

#endif
