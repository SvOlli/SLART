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
class MagicEncoderInterface;
class MySettings;
typedef QList<MagicEncoderInterface*> MagicEncoderList;


class MagicEncoderLoader
{
public:
   /*  */
   static MagicEncoderList tryLoading( QApplication *app = 0 );

private:
   MagicEncoderLoader();
   virtual ~MagicEncoderLoader();
   MagicEncoderLoader( const MagicEncoderLoader &other );
   MagicEncoderLoader &operator=( const MagicEncoderLoader &other );

   /*  */
   static MagicEncoderList tryLoading( const QDir &dir );
};

#endif /* MAGICENCODERLOADER_HPP */
