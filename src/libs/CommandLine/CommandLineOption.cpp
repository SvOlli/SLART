/*
 * src/libs/CommandLine/CommandLineOption.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CommandLineOption.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */



CommandLineOption::CommandLineOption( const QString &name, const QString &type, const QString &desc )
: mName( name )
, mType( type )
, mDesc( desc )
, mFail( false )
{
}


CommandLineOption::~CommandLineOption()
{
}


bool CommandLineOption::check()
{
   return mFail;
}


QString CommandLineOption::help()
{
   if( mType.isEmpty() )
   {
      return mName + ": " + mDesc;
   }
   else
   {
      return mName + " <" + mType + ">: " + mDesc;
   }
}
