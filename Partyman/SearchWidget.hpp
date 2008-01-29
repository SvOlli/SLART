/**
 * SearchWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SEARCHWIDGET_HPP
#define SEARCHWIDGET_HPP 1

#include <QWidget>


class QLineEdit;
class QLabel;
class QModelIndex;
class PlaylistWidget;
class PlaylistContentWidget;

class SearchWidget : public QWidget
{
Q_OBJECT

public:
   SearchWidget( PlaylistWidget *parent );
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

   PlaylistWidget        *mpPlaylist;
   PlaylistContentWidget *mpResults;
   QLineEdit             *mpInput;
   QLabel                *mpFound;
};

#endif
