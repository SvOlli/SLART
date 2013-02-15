/*
 * src/libs/Common/FileIconProvider.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "FileIconProvider.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


FileIconProvider::FileIconProvider()
: QFileIconProvider()
, mAudioIcon( ":/SLART/Icon.png" )
{
}


FileIconProvider::~FileIconProvider()
{
}


#if 0
QIcon FileIconProvider::icon( QFileIconProvider::IconType type ) const
{
   return QFileIconProvider::icon( type );
}
#endif


QIcon FileIconProvider::icon( const QFileInfo &info ) const
{
   if( !info.isFile() )
   {
      return QFileIconProvider::icon( info );
   }
   const QString suffix( info.suffix().toLower() );
   if( ( suffix == "ogg" ) ||
       ( suffix == "oga" ) ||
       ( suffix == "mp3" ) ||
       ( suffix == "flac" ) ||
       ( suffix == "wav") )
   {
      return mAudioIcon;
   }
   return QFileIconProvider::icon( info );
}


#if 0
QString FileIconProvider::type( const QFileInfo &info ) const
{
   return QFileIconProvider::type( info );
}
#endif
