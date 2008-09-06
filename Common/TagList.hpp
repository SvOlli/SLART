/**
 * TagList.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef TAGLIST_HPP
#define TAGLIST_HPP TAGLIST_HPP

#include <QStringList>


class MainWidget;

class TagList
{

public:
   TagList();

   /* return number of tags */
   int count() const { return mTags.count(); };
   /* clear all tags */
   void clear();
   /* read tag name */
   QString tagAt( int i ) const { return mTags.at(i); };
   /* read tag value */
   QString valueAt( int i ) const { return mValues.at(i); };
   /* set/add tag */
   void set( const QString &tag, const QString &value = QString() );
   /* get a tag */
   QString get( const QString &tag );
   /* generate filename according to tags */
   QString fileName( const QString &pattern, bool filterPath = true );
   /* normalize a string */
   static QString normalizeString( const QString &string );
   /* normalize a tag */
   QString normalizeTag( const QString &tag );
   
private:
   TagList( const TagList &other );
   TagList &operator=( const TagList &other );

   QStringList mTags;
   QStringList mValues;
};

#endif
