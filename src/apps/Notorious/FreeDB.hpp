/**
 * src/apps/Notorious/FreeDB.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FREEDB_HPP
#define FREEDB_HPP FREEDB_HPP

#include <QObject>

class QSqlDatabase;

class FreeDBQuery;

class FreeDB : public QObject
{
Q_OBJECT
   
public:
   FreeDB( QObject *parent = 0 );
   ~FreeDB();
   
private:
   FreeDB( const FreeDB &other );
   FreeDB &operator=( const FreeDB &other );

   QSqlDatabase *mpSqlDB;
   FreeDBQuery  *mpSearchQuery;
   FreeDBQuery  *mpInfoQuery;
};


#endif
