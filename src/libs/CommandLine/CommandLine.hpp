/**
 * src/libs/CommandLine/CommandLine.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP COMMANDLINE_HPP

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class CommandLineOption;


class CommandLine
{
public:
   CommandLine();
   virtual ~CommandLine();
   void option( const QString &name, const QString &desc, bool *value, bool setTo );
   void option( const QString &name, const QString &desc, QString *value );
#if 0
   void option( const QString &name, const QString &desc, long *value );
   void option( const QString &name, const QString &desc, QStringList *value );
   void option( const QString &name, const QString &desc, QList<long> *value );
#endif
   void parse( QStringList *nonOptions );
   bool check();
   QString help();
   
private:
   QList<CommandLineOption*>    mOptions;
};

#endif
