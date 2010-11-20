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

   /* add an option with a boolean value (enable/disable) */
   void option( const QString &name, const QString &desc, bool *value, bool setTo );
   /* add an option with a string parameter, new value replaces old */
   void option( const QString &name, const QString &desc, QString *value );
#if 0
   /* add an option with an integer parameter, new value replaces old */
   void option( const QString &name, const QString &desc, long *value );
   /* add an option with a string parameter, new value adds up to old ones */
   void option( const QString &name, const QString &desc, QStringList *value );
   /* add an option with an integer parameter, new value adds up to old ones */
   void option( const QString &name, const QString &desc, QList<long> *value );
#endif
   /* parse the list of commandline parameters returning non parameters */
   void parse( QStringList *nonOptions );
   /* check if parsing was without faults */
   bool check();
   /* generate help message of all options */
   QString help();

private:
   QList<CommandLineOption*>    mOptions;
};

#endif
