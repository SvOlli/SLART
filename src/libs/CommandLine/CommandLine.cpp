/**
 * src/libs/CommandLine/CommandLine.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "CommandLine.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
#include "CommandLineOptionBool.hpp"
#include "CommandLineOptionString.hpp"


CommandLine::CommandLine()
: mOptions()
{
}


CommandLine::~CommandLine()
{
   for( int i = 0; i < mOptions.count(); i++ )
   {
      delete mOptions.at(i);
   }
}


void CommandLine::parse( QStringList *nonOptions )
{
   QStringList args( QCoreApplication::arguments() );
   int arg = 0;
   int opt = 0;
   
   /* remove program name */
   args.removeFirst();

   /* remove all options with parameter */
   for( opt = 0; opt < mOptions.count(); opt++ )
   {
      for( arg = 0; arg < args.count() - 1; arg++ )
      {
         if( mOptions.at(opt)->parse( args.at(arg), args.at(arg+1) ) )
         {
            args.removeAt(arg);
            args.removeAt(arg);
            arg--;
         }
      }
   }

   /* remove all options without parameter */
   for( opt = 0; opt < mOptions.count(); opt++ )
   {
      for( arg = 0; arg < args.count(); arg++ )
      {
         if( mOptions.at(opt)->parse( args.at(arg) ) )
         {
            args.removeAt(arg);
            arg--;
         }
      }
   }

   if( nonOptions )
   {
      *nonOptions = args;
   }
}


QString CommandLine::help()
{
   QStringList messages;
   for( int opt = 0; opt < mOptions.count(); opt++ )
   {
      messages.append( mOptions.at(opt)->help() );
   }
   return messages.join( "\n" );
}


bool CommandLine::check()
{
   for( int opt = 0; opt < mOptions.count(); opt++ )
   {
      if( mOptions.at(opt)->check() )
      {
         return true;
      }
   }
   return false;
}


void CommandLine::option( const QString &name, const QString &desc, bool *value, bool setTo )
{
   mOptions.append( new CommandLineOptionBool( name, desc, value, setTo ) );
}


void CommandLine::option( const QString &name, const QString &desc, QString *value )
{
   mOptions.append( new CommandLineOptionString( name, desc, value ) );
}
