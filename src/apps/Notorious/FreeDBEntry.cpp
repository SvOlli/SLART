/**
 * src/apps/Notorious/FreeDBEntry.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FreeDBEntry.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


FreeDBEntry::FreeDBEntry()
: category()
, id()
, track(-1)
, title()
, playtime(-1)
#if INCLUDE_EXT
, ext()
#endif
{
}


FreeDBEntry::FreeDBEntry( const FreeDBEntry &other )
: category( other.category )
, id( other.id )
, track( other.track )
, title( other.title )
, playtime( other.playtime )
#if INCLUDE_EXT
, ext( other.ext )
#endif
{
}


#if INCLUDE_EXT
FreeDBEntry::FreeDBEntry( QString Category, QString ID, int Track, QString Title, int Playtime, QString Ext )
#else
FreeDBEntry::FreeDBEntry( QString Category, QString ID, int Track, QString Title, int Playtime )
#endif
: category( Category )
, id( ID )
, track( Track )
, title( Title )
, playtime( Playtime )
#if INCLUDE_EXT
, ext( Ext )
#endif
{
}


FreeDBEntry& FreeDBEntry::operator= (const FreeDBEntry& other)
{
   category = other.category;
   id       = other.id;
   track    = other.track;
   title    = other.title;
   playtime = other.playtime;
#if INCLUDE_EXT
   ext      = other.ext;
#endif
   return *this;
}
