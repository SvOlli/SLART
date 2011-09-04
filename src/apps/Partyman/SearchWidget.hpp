/**
 * src/apps/Partyman/SearchWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SEARCHWIDGET_HPP
#define SEARCHWIDGET_HPP SEARCHWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QLineEdit;
class QModelIndex;

/* forward declaration of local classes */
class Database;
class PlaylistContentWidget;
class PlaylistControlWidget;


class SearchWidget : public QWidget
{
   Q_OBJECT

public:
   SearchWidget( Database *database, PlaylistControlWidget *parent );
   virtual ~SearchWidget();

public slots:
   /* execute search */
   void search();
   /* do something with selected items */
   void selectedEntries( const QModelIndex &index, int key );
   /* handle focus */
   void setFocus();
   /* update the number of found entries */
   void updateCounter();

private:
   SearchWidget( const SearchWidget &that );
   SearchWidget &operator=( const SearchWidget &that );

   Database              *mpDatabase;
   PlaylistControlWidget *mpPlaylist;
   PlaylistContentWidget *mpResults;
   QLineEdit             *mpInput;
   QLabel                *mpFound;
};

#endif
