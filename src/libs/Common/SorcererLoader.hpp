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

class SorcererLoader //: public QObject
{
//Q_OBJECT

public:
   /*  */
   static void detect( QApplication *app, bool force = false );

private:
   /*  */
   static bool run( QApplication *app );
   /*  */
   static bool tryLoading( QApplication *app, const QDir &dir );

   SorcererLoader();
   virtual ~SorcererLoader();
   SorcererLoader( const SorcererLoader &other );
   SorcererLoader &operator=( const SorcererLoader &other );
};

#endif /* SORCERERLOADER_HPP */
