/*
 * src/tests/TestAppFileSysCompleter/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QApplication>
#include <QCompleter>
#include <QDir>
#include <QDirModel>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QTextStream>

/* local library headers */

/* local headers */


#define USE_DIRMODEL 0

int main(int argc, char *argv[])
{
   int retval = 0;

   QCoreApplication::setOrganizationName("SLART");
   QCoreApplication::setOrganizationDomain("svolli.org");
   QCoreApplication::setApplicationName("TestAppFileSysCompleter");
   QApplication app( argc, argv );

   QLineEdit *le = new QLineEdit();
   QCompleter *c = new QCompleter( le );
#if USE_DIRMODEL
   QDirModel *dm = new QDirModel( le );
   dm->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs );
   c->setModel( dm );
#else
   QFileSystemModel *fsm = new QFileSystemModel( le );
   fsm->setRootPath( "/" );
   c->setModel( fsm );
#endif
   le->setCompleter( c );

   le->setText( QDir::currentPath() );
   
   le->connect( le, SIGNAL(returnPressed()),
                &app, SLOT(quit()) );

   le->show();
   retval = app.exec();
   
   QTextStream qStdOut( stdout );
   qStdOut << le->text() << "\n";
   
   delete le;
   
   return retval;
}
