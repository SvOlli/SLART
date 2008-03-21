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
   int count() const { return mTags.count(); };
   void clear();
   QString tagAt( int i ) const { return mTags.at(i); };
   QString valueAt( int i ) const { return mValues.at(i); };
   void set( const QString &tag, const QString &value = QString() );
   QString fileName( const QString &pattern, bool filterPath = true );
   static QString normalizeString( const QString &string );
   QString normalizeTag( const QString &tag );
   
private:
   TagList( const TagList &other );
   TagList &operator=( const TagList &other );

   QStringList mTags;
   QStringList mValues;
};

#endif
