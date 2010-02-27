/**
 * src/libs/CommandLine/CommandLineOptionBool.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef COMMANDLINEOPTIONBOOL_HPP
#define COMMANDLINEOPTIONBOOL_HPP COMMANDLINEOPTIONBOOL_HPP

/* base class */
#include "CommandLineOption.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class CommandLineOptionBool : public CommandLineOption
{
public:
   CommandLineOptionBool( const QString &name, const QString &desc, bool *value, bool setTo );
   virtual bool parse( const QString &name );
   virtual bool parse( const QString &name, const QString &option );

private:
   bool *mValue;
   bool mSetTo;
};

#endif
