/**
 * src/apps/Notorious/FreeDBQuery.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FREEDBQUERY_HPP
#define FREEDBQUERY_HPP FREEDBQUERY_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QThread>
#include <QList>

/* local library headers */

/* local headers */
#include "FreeDBEntry.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */



class QString;
class Foundlist;

class FreeDBQuery : public QThread
{
Q_OBJECT
   
public:
   FreeDBQuery( Foundlist *parent = 0 );
   virtual ~FreeDBQuery();

   void run();
   void setQuery( const QString &query );
   void stop() { mBreak = true; };

   QList<FreeDBEntry> mEntries;

signals:
   void newData( int i );
   void running( bool r );

private:
   Foundlist  *mpParent;
   bool       mBreak;
   QString    mQuery;

};


#endif
