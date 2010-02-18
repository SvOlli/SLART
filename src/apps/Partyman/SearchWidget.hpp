/**
 * src/apps/Partyman/SearchWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SEARCHWIDGET_HPP
#define SEARCHWIDGET_HPP SEARCHWIDGET_HPP

#include <QWidget>

class QLabel;
class QLineEdit;
class QModelIndex;

class Database;
class PlaylistContentWidget;
class PlaylistWidget;


class SearchWidget : public QWidget
{
Q_OBJECT
   
public:
   SearchWidget( Database *database, PlaylistWidget *parent );
   virtual ~SearchWidget();
   
public slots:
   /* execute search */
   void search();
   /* do something with selected items */
   void selectedEntries( const QModelIndex &index, int key );
   /* handle focus */
   void setFocus();
   
private:
   SearchWidget( const SearchWidget &other );
   SearchWidget &operator=( const SearchWidget &other );
   
   Database              *mpDatabase;
   PlaylistWidget        *mpPlaylist;
   PlaylistContentWidget *mpResults;
   QLineEdit             *mpInput;
   QLabel                *mpFound;
};

#endif
