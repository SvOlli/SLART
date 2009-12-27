/**
 * FreeDBEntry.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FREEDBENTRY_HPP
#define FREEDBENTRY_HPP FREEDBENTRY_HPP

#include <QString>

class FreeDBEntry
{
public:
   FreeDBEntry();
   FreeDBEntry( const FreeDBEntry &other );
   FreeDBEntry( QString Category, QString ID, int Track, QString Title, int Playtime, QString Ext );
   FreeDBEntry& operator=(const FreeDBEntry& other);
   
   QString category;
   QString id;
   int     track;
   QString title;
   int     playtime;
   QString ext;
};

#endif
