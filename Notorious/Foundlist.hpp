/**
 * Foundlist.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FOUNDLIST_HPP
#define FOUNDLIST_HPP FOUNDLIST_HPP

#include <QTableWidget>
#include "FreeDBQuery.hpp"


class Foundlist : public QTableWidget
{
Q_OBJECT
   
public:
   Foundlist( QWidget *parent = 0 );
   virtual ~Foundlist();

public slots:
   void handleNewData( int i );
   void handleClicked( QTableWidgetItem *clickedItem );
   void runQuery( const QString &query );

signals:
   void clicked( const QString &category, const QString &id );
   void queryRunning( bool r );

private:
   Foundlist( const Foundlist &other );
   Foundlist &operator=( const Foundlist &other );
   
   FreeDBQuery mQuery;
};


#endif
