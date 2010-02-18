/**
 * src/apps/Notorious/FreeDBEntry.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "FreeDBEntry.hpp"


FreeDBEntry::FreeDBEntry()
: category()
, id()
, track(-1)
, title()
, playtime(-1)
, ext()
{
}


FreeDBEntry::FreeDBEntry( const FreeDBEntry &other )
: category( other.category )
, id( other.id )
, track( other.track )
, title( other.title )
, playtime( other.playtime )
, ext( other.ext )
{
}


FreeDBEntry::FreeDBEntry( QString Category, QString ID, int Track, QString Title, int Playtime, QString Ext )
: category( Category )
, id( ID )
, track( Track )
, title( Title )
, playtime( Playtime )
, ext( Ext )
{
}


FreeDBEntry& FreeDBEntry::operator= (const FreeDBEntry& other)
{
   category = other.category;
   id       = other.id;
   track    = other.track;
   title    = other.title;
   playtime = other.playtime;
   ext      = other.ext;
   return *this;
}
