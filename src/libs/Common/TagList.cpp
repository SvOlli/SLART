/**
 * src/libs/Common/TagList.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "TagList.hpp"
#include "MySettings.hpp"

#include <QApplication>

#include "Trace.hpp"


TagList::TagList()
{
}


void TagList::set( const QString &tag, const QString &value )
{
   int i = mTags.indexOf( tag.toUpper() );
   
   if( i < 0 )
   {
      /* new element */
      if( !value.isEmpty() )
      {
         mTags.append( tag.toUpper() );
         mValues.append( value );
      }
   }
   else
   {
      /* element found */
      if( value.isEmpty() )
      {
         mTags.removeAt( i );
         mValues.removeAt( i );
      }
      else
      {
         mValues.replace( i, value );
      }
   }
}


QString TagList::get( const QString &tag )
{
   for( int i = 0; i < mTags.count(); i++ )
   {
      if( mTags.at(i) == tag.toUpper() )
      {
         return mValues.at(i);
      }
   }
   return QString();
}


void TagList::clear()
{
   mTags.clear();
   mValues.clear();
}


QString TagList::fileName( const QString &pattern, bool filterPath )
{
   QStringList parts( pattern.split( "|", QString::SkipEmptyParts ) );
   QString filename;
   int i;
   int j;
   
   for( i = 0; i < parts.size() ; i++ )
   {
      switch( parts.at(i).at(0).unicode() )
      {
         case '$':
            j = mTags.indexOf( parts.at(i).toUpper().mid(1) );
            if( (j < 0) && (parts.at(i).toUpper() == "$YEAR") )
            {
               j = mTags.indexOf( "DATE" );
            }
               
            if( j >= 0 )
            {
               QString value( mValues.at(j) );
               if( filterPath )
               {
                  value.remove( QRegExp("[:?]") );
                  value.replace( QRegExp("[\"\\\\|/\\*]"), "_" );
               }
               
               filename.append( value );
            }
            break;
         case '#':
            j = mTags.indexOf( parts.at(i).toUpper().mid(2) );
            if( j >= 0 )
            {
               bool ok;
               int size = parts.at(i).mid(1,1).toInt( &ok );
               
               if( !ok ) break;
               int value = 1000000000 + mValues.at(j).toInt( &ok );
               
               if( !ok ) break;
               filename.append( QString::number(value).right(size) );
            }
            break;
         default:
            filename.append( parts.at(i) );
            break;
      }
   }
   
   return filename;
}


QString TagList::normalizeString( const QString &string )
{
   QString newString;
   MySettings settings( "Global" );
   
   if( settings.value( "NormalizeCase", false ).toBool() )
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
   newString.replace( QString::fromLatin1( "\0xB4" ), "'" ); // accent acute
   newString.replace( "\0x60", "'" );                        // accent grave
   if( settings.value( "NormalizeSpaces", false ).toBool() )
   {
      newString = newString.simplified();
   }
   
   return newString;
}


QString TagList::normalizeTag( const QString &tag )
{
   int id = mTags.indexOf( tag.toUpper() );
   
   if( id < 0 )
   {
      return QString();
   }
   
   QString value( normalizeString(mValues.at(id)) );
   
   mValues.replace( id, value );
   
   return value;
}