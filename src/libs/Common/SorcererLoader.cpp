/*
 * src/libs/Common/SorcererLoader.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SorcererLoader.hpp"

/* system headers */
#include <cstdlib> // exit(), getenv()

/* Qt headers */
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <QProcess>
#include <QStringList>

/* local library headers */

/* local headers */
#include "DatabaseInterface.hpp"
#include "Settings.hpp"
#include "Version.hpp"


void SorcererLoader::detect()
{
   QStringList regVersions( Settings::value( Settings::GlobalVersion ).split('.') );
   QStringList thisVersions( QString( SLART_VERSION ).split('.') );
   unsigned long regVersion = 0;
   unsigned long thisVersion = 0;

   for( int i = 0; i < (int)sizeof(unsigned long); i++ )
   {
      regVersion <<= 8;
      thisVersion <<= 8;
      if( i < regVersions.size() )
      {
         regVersion += regVersions.at(i).toULong();
      }
      if( i < thisVersions.size() )
      {
         thisVersion += thisVersions.at(i).toULong();
      }
   }

   if( !Settings::exist() || !DatabaseInterface::exists() )
   {
      QStringList args;
      args << QApplication::applicationName();
      if( QProcess::execute( "Sorcerer", args ) )
      {
         QMessageBox::critical( 0, QApplication::applicationName(),
                                QObject::tr("Setup wizard did not complete successfully.") );
         ::exit( 1 );
      }
   }

   if( regVersion < thisVersion )
   {
      if( Settings::cleanup() )
      {
         QMessageBox::information( 0, QApplication::applicationName(),
                                   QObject::tr("Cleaned up unused setting entries.") );
      }
   }
}
