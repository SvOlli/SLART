/**
 * src/apps/Notorious/FreeDBEntry.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FREEDBENTRY_HPP
#define FREEDBENTRY_HPP FREEDBENTRY_HPP

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class FreeDBEntry
{
public:
   FreeDBEntry();
   FreeDBEntry( const FreeDBEntry &other );
#if INCLUDE_EXT
   FreeDBEntry( QString Category, QString ID, int Track, QString Title, int Playtime, QString Ext );
#else
   FreeDBEntry( QString Category, QString ID, int Track, QString Title, int Playtime );
#endif
   FreeDBEntry& operator=(const FreeDBEntry& other);
   
   QString category;
   QString id;
   int     track;
   QString title;
   int     playtime;
#if INCLUDE_EXT
   QString ext;
#endif
};

#endif
