/*
 * src/libs/CommandLine/CommandLineOptionBool.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CommandLineOptionBool.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


CommandLineOptionBool::CommandLineOptionBool( const QString &name, const QString &desc, bool *value, bool setTo )
: CommandLineOption( name, "", desc )
, mValue( value )
, mSetTo( setTo )
{
}


CommandLineOptionBool::~CommandLineOptionBool()
{
}


bool CommandLineOptionBool::parse( const QString &name )
{
   if( name == mName )
   {
      *mValue = mSetTo;
      return true;
   }
   return false;
}


bool CommandLineOptionBool::parse( const QString &name, const QString &option )
{
   Q_UNUSED(name);
   Q_UNUSED(option);
   return false;
}
