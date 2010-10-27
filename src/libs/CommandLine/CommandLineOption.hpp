/**
 * src/libs/CommandLine/CommandLineOption.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef COMMANDLINEOPTION_HPP
#define COMMANDLINEOPTION_HPP COMMANDLINEOPTION_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class CommandLineOption
{
public:
   CommandLineOption( const QString &name, const QString &type, const QString &desc );
   virtual ~CommandLineOption();

   /* try to parse option name without argument */
   virtual bool parse( const QString &name ) = 0;
   /* try to parse option name with an argument */
   virtual bool parse( const QString &name, const QString &option ) = 0;
   /* check if parsing was successful */
   virtual bool check();
   /* generate help message for this option */
   virtual QString help();

   const QString mName;
   const QString mType;
   const QString mDesc;

protected:
   bool          mFail;

private:
   CommandLineOption( const CommandLineOption &that );
   CommandLineOption &operator=( const CommandLineOption &that );
};

#endif
