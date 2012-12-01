/*
 * src/libs/Common/LEDIcon.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef LEDICON_HPP
#define LEDICON_HPP LEDICON_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QColor>
#include <QImage>
#include <QPixmap>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief class for creating an image of an LED

 It can be customized in size and color.
 Here is an example with size=50 and color=green: \image html led_50.svg

*/
class LEDIcon
{
public:
   /*!
    \brief constructor

    \param color
    \param size
   */
   LEDIcon( const QColor &color, int size = 50 );
   /*!
    \brief destructor

   */
   virtual ~LEDIcon();
   /*!
    \brief return current size

    \return int
   */
   int size() const;
   /*!
    \brief set size

    \param size
   */
   void setSize( int size );
   /*!
    \brief return current color value

    \return QColor
   */
   QColor color() const;
   /*!
    \brief set new color value

    \param color
   */
   void setColor( const QColor &color );
   /*!
    \brief create led as QImage

    \return QImage
   */
   QImage toImage() const;
   /*!
    \brief create led as QPixmap

    \return QPixmap
   */
   QPixmap toPixmap() const;
   /*!
    \brief create image without the need of creating an object first

    \param color
    \return QImage
   */
   static QImage image( const QColor &color, int size = 50 );
   /*!
    \brief create pixmap without the need of creating an object first

    \param color
    \return QPixmap
   */
   static QPixmap pixmap( const QColor &color, int size = 50 );

private:
   Q_DISABLE_COPY( LEDIcon )

   int            mSize; /*!< current size */
   QColor         mColor; /*!< current color */

   static QString data( const QColor &color, int size );
};

/*! @} */

#endif
