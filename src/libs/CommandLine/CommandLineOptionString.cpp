/**
 * src/libs/CommandLine/CommandLineOptionString.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CommandLineOptionString.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


CommandLineOptionString::CommandLineOptionString( const QString &name, const QString &desc, QString *value )
: CommandLineOption( name, "string", desc )
, mValue( value )
{
}


bool CommandLineOptionString::parse( const QString &name )
{
   if( name == mName )
   {
      mFail = true;
   }
   return false;
}


bool CommandLineOptionString::parse( const QString &name, const QString &option )
{
   if( name == mName )
   {
      *mValue = option;
      mFail = false;
      return true;
   }
   else
   {
      return false;
   }
}


QString CommandLineOptionString::help()
{
   if( mValue->isEmpty() )
   {
      return CommandLineOption::help();
   }
   else
   {
      return CommandLineOption::help() + QString(" (default: \"%1\")").arg( *mValue );
   }
}
