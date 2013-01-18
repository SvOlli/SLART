/*
 * src/libs/Common/TagMap.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "TagMap.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "Settings.hpp"

/* class variables */

#include "Trace.hpp"


QString TagMap::fileName( const QString &pattern, bool filterPath )
{
   QStringList parts( pattern.split( "|", QString::SkipEmptyParts ) );
   QString filename;
   QString val;
   const QString empty;

   foreach( const QString &part, parts )
   {
      switch( part.at(0).unicode() )
      {
      case '$':
         val = value( part.toUpper().mid(1).toAscii(), empty );
         if( val.isNull() && (part.toUpper() == "$YEAR") )
         {
            val = value( "DATE", empty );
         }

         if( !val.isNull() )
         {
            if( filterPath )
            {
               val.remove( QRegExp("[:?]") );
               val.replace( QRegExp("[\"\\\\|/\\*]"), "_" );
            }

            filename.append( val );
         }
         break;
      case '#':
         val = value( part.toUpper().mid(2).toAscii(), empty );

         if( !val.isNull() )
         {
            bool ok;
            int size = part.mid(1,1).toInt( &ok );

            if( !ok ) break;
            int intval = 1000000000 + val.toInt( &ok );

            if( !ok ) break;
            filename.append( QString::number(intval).right(size) );
         }
         break;
      default:
         filename.append( part );
         break;
      }
   }

   return filename;
}


QString TagMap::normalizeString( const QString &string )
{
   QString newString;

   if( Settings::value( Settings::GlobalNormalizeCase ) )
   {
      bool nextUpper = true;

      for( int i = 0; i < string.size(); i++ )
      {
         if( nextUpper )
         {
            newString.append(string.at(i).toUpper());
         }
         else
         {
            newString.append(string.at(i).toLower());
         }

         switch( newString.at(i).toAscii() )
         {
            case ' ':
            case '.':
            case '-':
               nextUpper = true;
               break;
            case '(':
            case ')':
            case '[':
            case ']':
            case '\'':
            case '"':
               break;
            default:
               nextUpper = false;
               break;
         }
      }
   }
   else
   {
      newString = string;
   }

   newString.replace( "\"", "''" );
   newString.replace( QChar::fromLatin1( '\xB4' ), '\'' ); // accent acute
   newString.replace( QChar::fromLatin1( '\x60' ), '\'' ); // accent grave
   if( Settings::value( Settings::GlobalNormalizeSpaces ) )
   {
      newString = newString.simplified();
   }

   return newString;
}


MyLuaTable TagMap::toLuaTable()
{
   MyLuaTable retval;
   foreach( const QByteArray &tag, keys() )
   {
      retval.insert( tag.toLower(), value( tag ) );
   }
   return retval;
}
