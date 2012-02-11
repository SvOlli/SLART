/*
 * src/apps/Notorious/Foundlist.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FOUNDLIST_HPP
#define FOUNDLIST_HPP FOUNDLIST_HPP

/* base class */
#include <QTableWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "FreeDBQuery.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Notorious

  @{
*/

/*!
 \brief TODO

 \dotfile "graphs/apps/Karmadrome/Foundlist_connect.dot" "Connect Graph"
*/
class Foundlist : public QTableWidget
{
   Q_OBJECT

public:
   Foundlist( QWidget *parent = 0 );
   virtual ~Foundlist();

public slots:
   void handleNewData( int i );
   void handleClicked( QTableWidgetItem *clickedItem );
   void runQuery( const QString &query, const QString &category = QString() );

signals:
   void clicked( const QString &category, const QString &id );
   void queryRunning( bool r );

private:
   Foundlist( const Foundlist &that );
   Foundlist &operator=( const Foundlist &that );

   FreeDBQuery mQuery;
};

/*! @} */

#endif
