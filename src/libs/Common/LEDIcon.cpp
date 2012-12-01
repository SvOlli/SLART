/*
 * src/libs/Common/LEDIcon.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "LEDIcon.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QFile>
#include <QPixmap>

/* local library headers */

/* local headers */


LEDIcon::LEDIcon(const QColor &color, int size )
: mSize( size )
, mColor( color )
{
}


LEDIcon::~LEDIcon()
{
}


QColor LEDIcon::color() const
{
   return mColor;
}


void LEDIcon::setColor( const QColor &color )
{
   mColor = color;
}


int LEDIcon::size() const
{
   return mSize;
}


void LEDIcon::setSize( int size )
{
   mSize = size;
}


QImage LEDIcon::toImage() const
{
   return QImage::fromData( data( mColor, mSize ).toAscii(), "SVG" );
}


QPixmap LEDIcon::toPixmap() const
{
   QPixmap pm;
   pm.loadFromData( data( mColor, mSize ).toAscii(), "SVG" );
   return pm;
}


QImage LEDIcon::image(const QColor &color, int size )
{
   return LEDIcon( color, size ).toImage();
}


QPixmap LEDIcon::pixmap(const QColor &color, int size )
{
   return LEDIcon( color, size ).toPixmap();
}


QString LEDIcon::data(const QColor &color, int size)
{
   QString size0( QString::number(size) );
   QString size1( QString::number(0.625*size) );
   QString size2( QString::number(14*size) );
   QString size3( QString::number(28*size) );
   return QString(
            // based upon image found at http://www.clker.com/clipart-led-green-2.html
            // Thanks to marxa (http://www.clker.com/profile-70069.html)
            // hand optimized and added scaling parameter by SvOlli
            "<?xml version='1.0'?><svg width='%1' height='%2' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'>"
             "<defs>"
              "<linearGradient y2='0.154184' y1='1.14599' xlink:href='#g4' x2='0.567019' x1='0.577658' id='g1'/>"
              "<linearGradient y2='0.847627' y1='0.057144' xlink:href='#g5' x2='0.609527' x1='0.533336' id='g2'/>"
              "<linearGradient y2='1' y1='0.018268' xlink:href='#g6' x2='0.500001' x1='0.495689' id='g3'/>"
              "<linearGradient id='g4'>"
               "<stop stop-color='#ffffff' stop-opacity='0' offset='0'/>"
               "<stop stop-color='#ffffff' stop-opacity='0.87451' offset='1'/>"
              "</linearGradient>"
              "<linearGradient id='g5'>"
               "<stop stop-color='#828282' offset='0'/>"
               "<stop stop-color='#929292' stop-opacity='0.352941' offset='1'/>"
              "</linearGradient>"
              "<linearGradient id='g6'>"
               "<stop stop-color='#adadad' offset='0'/>"
               "<stop stop-color='#f0f0f0' offset='1'/>"
              "</linearGradient>"
             "</defs>"
             "<g transform='matrix(%3,0,0,%4,-%5,-%6)'>"
              "<path fill='url(#g3)' stroke-width='0.8' stroke-linecap='round' stroke-linejoin='round' stroke-miterlimit='4' d='m23.999998,45.599998c0,0.441601 -0.3584,0.799999 -0.799999,0.799999c-0.441601,0 -0.800001,-0.358398 -0.800001,-0.799999c0,-0.441601 0.3584,-0.799999 0.800001,-0.799999c0.441599,0 0.799999,0.358398 0.799999,0.799999z'/>"
              "<path fill='url(#g2)' stroke-width='0.8' stroke-linecap='round' stroke-linejoin='round' stroke-miterlimit='4' d='m23.845621,45.599995c0,0.356384 -0.289236,0.645622 -0.645618,0.645622c-0.356384,0 -0.645622,-0.289238 -0.645622,-0.645622c0,-0.356373 0.289238,-0.645607 0.645622,-0.645607c0.356382,0 0.645618,0.289234 0.645618,0.645607z'/>"
              "<path fill='%7' stroke-width='0.8' stroke-linecap='round' stroke-linejoin='round' stroke-miterlimit='4' d='m23.777987,45.599995c0,0.31905 -0.258938,0.577988 -0.577984,0.577988c-0.31905,0 -0.577986,-0.258938 -0.577986,-0.577988c0,-0.319035 0.258936,-0.577972 0.577986,-0.577972c0.319046,0 0.577984,0.258938 0.577984,0.577972z'/>"
              "<path fill='url(#g1)' stroke-width='0.8' stroke-linecap='round' stroke-linejoin='round' stroke-miterlimit='4' d='m23.660416,45.407204c0,0.196857 -0.206598,0.356628 -0.461157,0.356628c-0.25456,0 -0.461159,-0.159771 -0.461159,-0.356628c0,-0.196854 0.206598,-0.356625 0.461159,-0.356625c0.254559,0 0.461157,0.159771 0.461157,0.356625z'/>"
             "</g>"
            "</svg>"
            ).arg( size0, size0, size1, size1, size2, size3, color.name() );
}
