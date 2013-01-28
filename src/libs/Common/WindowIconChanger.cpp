/*
 * src/libs/Common/WindowIconChanger.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "WindowIconChanger.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QTimer>
#include <QWidget>

/* local library headers */

/* local headers */


WindowIconChanger::WindowIconChanger( QWidget *widget, const QIcon &icon,
                                      QObject *parent )
: QObject( parent )
, mpWidget( widget )
, mpTimer( new QTimer( this ) )
, mNewIcon( icon )
, mOldIcon()
{
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(revert()) );
}


WindowIconChanger::~WindowIconChanger()
{
}


QIcon WindowIconChanger::newIcon()
{
   return mNewIcon;
}


void WindowIconChanger::setNewIcon( const QIcon &icon )
{
   mNewIcon = icon;
}


void WindowIconChanger::changed( bool enable )
{
   if( enable )
   {
      if( mpWidget->windowIcon().cacheKey() != mNewIcon.cacheKey() )
      {
         mOldIcon = mpWidget->windowIcon();
         mpWidget->setWindowIcon( mNewIcon );
      }
   }
   else
   {
      mpTimer->start( 150 );
   }
}

void WindowIconChanger::revert()
{
   if( mpWidget->windowIcon().cacheKey() == mNewIcon.cacheKey() )
   {
      mpWidget->setWindowIcon( mOldIcon );
   }
}
