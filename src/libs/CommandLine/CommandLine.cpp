/**
 * src/libs/CommandLine/CommandLine.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include <QCoreApplication>
#include <QStringList>

#include "CommandLine.hpp"
#include "CommandLineOptionBool.hpp"
#include "CommandLineOptionString.hpp"

#include <QtDebug>


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