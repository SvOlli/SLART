/**
 * src/libs/Common/SorcererLoader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERLOADER_HPP
#define SORCERERLOADER_HPP SORCERERLOADER_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QApplication;
class QDir;
class QPluginLoader;

/* forward declaration of local classes */
class SorcererInterface;
class MySettings;


class SorcererLoader
{
public:
   /* detect if Sorcerer needs to be loaded and run */
   static void detect( bool force = false );

private:
   SorcererLoader();
   virtual ~SorcererLoader();
   SorcererLoader( const SorcererLoader &that );
   SorcererLoader &operator=( const SorcererLoader &that );

   /* search for Sorcerer and try loading it */
   /* try to load Sorcerer from the specified dir */
   static SorcererInterface *tryLoading();
   static SorcererInterface *tryLoading( const QDir &dir );
   /* remove Sorcerer from memory */
   static void unload();

   static QPluginLoader *cpPluginLoader; /*!< \brief used for loading Sorcerer */
};

#endif /* SORCERERLOADER_HPP */
