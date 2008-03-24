/**
 * PlaylistContentWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYLISTCONTENTWIDGET_HPP
#define PLAYLISTCONTENTWIDGET_HPP PLAYLISTCONTENTWIDGET_HPP

#include <QListWidget>


class PlaylistContentWidget : public QListWidget
{
Q_OBJECT
   
public:
   PlaylistContentWidget( bool allowResort, QWidget *parent = 0 );

public slots:
   /* remove selected items from list widget and put them into the list parameter */
   void removeSelectedItems( QStringList *list = 0 );
   /* move items for reordering list */
   void moveItem( QListWidgetItem *item, QListWidgetItem *pos );
   /* set the tool tip to full entry */
   void setToolTip( const QModelIndex &index );
   /* handle single click */
   void handleClick( QListWidgetItem *item );
   /* handle double click */
   void handleDoubleClick( QListWidgetItem *item );

protected:
   /* catch right mouse button */
   virtual void contextMenuEvent( QContextMenuEvent *event );
   /* for reordering of list */
   void mouseReleaseEvent(QMouseEvent *event);
   /* for reordering of list */
   void mouseMoveEvent(QMouseEvent *event);
   /* for handling key presses */
   virtual void keyPressEvent( QKeyEvent *event );

signals:
   void context( const QModelIndex &index, int key );

private:
   PlaylistContentWidget( const PlaylistContentWidget &other );
   PlaylistContentWidget &operator=( const PlaylistContentWidget &other );

   bool mMovingItem;
   bool mLeftButton;
};

#endif
