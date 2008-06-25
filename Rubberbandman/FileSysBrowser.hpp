/**
 * FileSysBrowser.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSBROWSER_HPP
#define FILESYSBROWSER_HPP FILESYSBROWSER_HPP

#include <QWidget>

//class FileSysTreeView;
class QTreeView;
class QDirModel;
class QLineEdit;
class QPushButton;
class QModelIndex;
class QTimer;
class QShowEvent;
class Database;


class FileSysBrowser : public QWidget
{
Q_OBJECT

public:
   FileSysBrowser( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* get the index of clicked entry */
   void entryClicked( const QModelIndex &index );
   /* change the root dir */
   void handleRootDir();
   /* handle dir up button */
   void handleDotButton();
   /* handle the set-back timer */
   void handleTimer();
   /* show a filename in browser */
   void scrollTo( const QString &fileName );
   /* handle right mouse button (send track name to Partyman) */
   void contextMenu( const QPoint &pos );

signals:
   /* emit the path of clicked entry */
   void clicked( const QString &path );

private:
   FileSysBrowser( const FileSysBrowser &other );
   FileSysBrowser &operator=( const FileSysBrowser &other );

   Database     *mpDatabase;
   QLineEdit    *mpRootDir;
   QPushButton  *mpSetButton;
   QPushButton  *mpDotButton;
   QTimer       *mpTimer;
   QTreeView    *mpView;
   QDirModel    *mpModel;
};

#endif
