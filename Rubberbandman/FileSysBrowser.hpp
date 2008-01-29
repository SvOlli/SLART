/**
 * FileSysBrowser.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSBROWSER_HPP
#define FILESYSBROWSER_HPP FILESYSBROWSER_HPP

#include <QWidget>
#include <QDir>
class QString;

//class FileSysTreeView;
class QTreeView;
class FileSysTreeModel;
class QModelIndex;

class FileSysBrowser : public QWidget
{
Q_OBJECT

public:
   FileSysBrowser( const QDir    &rootDir,  QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   FileSysBrowser( const QString &rootPath, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void entryClicked( const QModelIndex &index );

signals:
   void clicked( const QString &path );

private:
   FileSysBrowser( const FileSysBrowser &other );
   FileSysBrowser &operator=( const FileSysBrowser &other );

   void construct();

   QDir mRootDir;
//   FileSysTreeView  *mpFileSysTreeView;
   QTreeView        *mpFileSysTreeView;
   FileSysTreeModel *mpFileSysTreeModel;
};

#endif
