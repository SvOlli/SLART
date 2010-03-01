/**
 * src/libs/CommandLine/SorcererLoader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERLOADER_HPP
#define SORCERERLOADER_HPP

//#include <QObject>

class QApplication;
class QDir;
class QString;
class QVariant;

class MySettings;

class SorcererLoader
{
public:
   /*  */
   static void detect( QApplication *app, bool force = false );

private:
   SorcererLoader();
   virtual ~SorcererLoader();
   SorcererLoader( const SorcererLoader &other );
   SorcererLoader &operator=( const SorcererLoader &other );

   /*  */
   static bool run( QApplication *app );
   /*  */
   static bool tryLoading( QApplication *app, const QDir &dir );
   /*  */
   static void cleanupSettings( bool withDefaults = false );
   /*  */
   static void cleanupSettings( MySettings *settings );
   /*  */
   static void setDefault( MySettings *settings, const QString &name,
                           const QVariant &value );
};

#endif /* SORCERERLOADER_HPP */
