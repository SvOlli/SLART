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

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QImage;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

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
    \brief \todo

    \return QSize
   */
   virtual QSize minimumSizeHint() const;

protected:
   /*!
    \brief intercepting paint event for scaled drawing

   */
   void paintEvent( QPaintEvent *event );

public slots:
   /*!
    \brief set the image data

   */
   void setImage( const QImage &image );
   /*!
    \brief set the image data

   */
   void setImage( const QByteArray &data );
   /*!
    \brief load image data from file

   */
   void setImage( const QString &fileName );
   /*!
    \brief set aspect ratio: Qt::IgnoreAspectRatio, Qt::KeepAspectRatio, Qt::KeepAspectRatioByExpanding

   */
   void setAspectRatioMode( Qt::AspectRatioMode mode );
   /*!
    \brief set transformation mode: Qt::FastTransformation, Qt::SmoothTransformation

   */
   void setTransformationMode( Qt::TransformationMode mode );

signals:
   void currentRatio( float radio );

private:
   Q_DISABLE_COPY( ImageWidget )

   Qt::AspectRatioMode     mAspectRatioMode;
   Qt::TransformationMode  mTransformationMode;
   QImage                  *mpImage;
};

/*! @} */

#endif
