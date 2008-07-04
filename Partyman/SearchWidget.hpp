/**
 * SearchWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SEARCHWIDGET_HPP
#define SEARCHWIDGET_HPP SEARCHWIDGET_HPP

#include <QWidget>


class QLineEdit;
class QLabel;
class QModelIndex;
class PlaylistWidget;
class PlaylistContentWidget;
class Database;

class SearchWidget : public QWidget
{
Q_OBJECT

public:
   SearchWidget( Database *database, PlaylistWidget *parent );
   virtual ~SearchWidget();
   QLineEdit *getLineEdit();

public slots:
   /* execute search */
   void search();
   /* do something with selected items */
   void selectedEntries( const QModelIndex &index, int key );
   /* handle focus */
   void setFocus();

signals:

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
