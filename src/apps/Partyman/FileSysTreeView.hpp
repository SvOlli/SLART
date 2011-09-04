/**
 * src/apps/Partyman/FileSysTreeView.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FILESYSTREEWIDGET_HPP
#define FILESYSTREEWIDGET_HPP FILESYSTREEWIDGET_HPP

/* base class */
#include <QTreeView>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class FileSysTreeView : public QTreeView
{
   Q_OBJECT

public:
   FileSysTreeView( QWidget *parent = 0 );
   virtual ~FileSysTreeView();

protected:
   /* handle right mouse button */
   virtual void contextMenuEvent( QContextMenuEvent *event );
   /* handle return/enter key */
   virtual void keyPressEvent( QKeyEvent *event );

signals:
   /* emit the request for enqueueing */
   void context( const QModelIndex &index );

private slots:
   /* emit add request if the index represents a file */
   void addIfFile( const QModelIndex &index );
   /* set the tool tip if an entry is truncated */
   void setToolTip( const QModelIndex &index );

private:
   FileSysTreeView( const FileSysTreeView &that );
   FileSysTreeView &operator=( const FileSysTreeView &that );
};

#endif
