/**
 * src/libs/CommandLine/CommandLineOptionString.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef COMMANDLINEOPTIONSTRING_HPP
#define COMMANDLINEOPTIONSTRING_HPP COMMANDLINEOPTIONSTRING_HPP

/* base class */
#include "CommandLineOption.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class CommandLineOptionString : public CommandLineOption
{
public:
   CommandLineOptionString( const QString &name, const QString &desc, QString *value );
   virtual bool parse( const QString &name );
   virtual bool parse( const QString &name, const QString &option );

private:
   QString *mValue;
};

#endif
