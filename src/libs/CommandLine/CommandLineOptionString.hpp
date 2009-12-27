/**
 * src/libs/CommandLine/CommandLineOptionString.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef COMMANDLINEOPTIONSTRING_HPP
#define COMMANDLINEOPTIONSTRING_HPP COMMANDLINEOPTIONSTRING_HPP

#include "CommandLineOption.hpp"

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
