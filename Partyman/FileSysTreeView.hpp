/**
 * FileSysTreeView.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSTREEWIDGET_HPP
#define FILESYSTREEWIDGET_HPP 1

#include <QTreeView>


class FileSysTreeView : public QTreeView
{
Q_OBJECT
   
public:
   FileSysTreeView( QWidget *parent = 0 );

protected:
   /* handle right mouse button */
   virtual void contextMenuEvent( QContextMenuEvent *event );
   /* handle return/enter key */
   virtual void keyPressEvent( QKeyEvent *event );

signals:
   void context( const QModelIndex &index );

private slots:
   void addIfFile( const QModelIndex &index );
   void setToolTip( const QModelIndex &index );

private:
   FileSysTreeView( const FileSysTreeView &other );
   FileSysTreeView &operator=( const FileSysTreeView &other );
};

#endif
