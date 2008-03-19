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


class FileSysBrowser : public QWidget
{
Q_OBJECT

public:
   FileSysBrowser( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void entryClicked( const QModelIndex &index );
   void handleRootDir();
   void handleTimer();

signals:
   void clicked( const QString &path );
   void clickedDir( const QString &path );

private:
   FileSysBrowser( const FileSysBrowser &other );
   FileSysBrowser &operator=( const FileSysBrowser &other );

   QLineEdit    *mpRootDir;
   QPushButton  *mpSetButton;
   QTimer       *mpTimer;
   QTreeView    *mpView;
   QDirModel    *mpModel;
};

#endif
