/**
 * src/libs/Common/MagicEncoderLoader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERLOADER_HPP
#define MAGICENCODERLOADER_HPP MAGICENCODERLOADER_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QApplication;
class QDir;
class QPluginLoader;

/* forward declaration of local classes */
class MagicEncoderProxy;
class MySettings;
typedef QList<MagicEncoderProxy*> MagicEncoderList;


class MagicEncoderLoader
{
public:
   /* function to load MagicEncoders */
   static MagicEncoderList tryLoading( const QString &msgHeader,
                                       QApplication *app = 0 );

private:
   MagicEncoderLoader();
   virtual ~MagicEncoderLoader();
   MagicEncoderLoader( const MagicEncoderLoader &other );
   MagicEncoderLoader &operator=( const MagicEncoderLoader &other );

   /* internal function to load MagicEncoders */
   static MagicEncoderList tryLoading( const QString &msgHeader, const QDir &dir );
};

#endif /* MAGICENCODERLOADER_HPP */
