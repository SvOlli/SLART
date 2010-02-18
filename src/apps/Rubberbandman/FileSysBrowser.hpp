/**
 * src/apps/Rubberbandman/FileSysBrowser.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FILESYSBROWSER_HPP
#define FILESYSBROWSER_HPP FILESYSBROWSER_HPP

#include <QWidget>

#include <QModelIndex>
#include <QFileInfo>

class QAction;
class QDirModel;
class QLineEdit;
class QPushButton;
class QModelIndex;
class QShowEvent;
class QTreeView;

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
   /* show a filename in browser */
   void scrollTo( const QString &fileName );
   /* handle right mouse button (send track name to Partyman) */
   void contextMenu( const QPoint &pos );
   /* handle menu entry "Send To Partyman" */
   void menuSendToPartyman();
   /* handle menu entries "Move" and "Move Directory" */
   void menuMove( bool withContent = false );
   /* handle menu entry "Move Content" with wrapper for menuMove() */
   void menuMoveContent();
   /* handle menu entry "Rename" */
   void menuRename();
   /* handle menu entry "Delete" */
   void menuDelete();
   
signals:
   /* emit the path of clicked entry */
   void clicked( const QString &path );
   
private:
   FileSysBrowser( const FileSysBrowser &other );
   FileSysBrowser &operator=( const FileSysBrowser &other );
   
   Database     *mpDatabase;
   QLineEdit    *mpRootDir;
   QPushButton  *mpDotButton;
   QTreeView    *mpView;
   QDirModel    *mpModel;
   QAction      *mpMenuSendToPartyman;
   QAction      *mpMenuRescan;
   QAction      *mpMenuMoveFile;
   QAction      *mpMenuMoveDirectory;
   QAction      *mpMenuMoveContent;
   QAction      *mpMenuRename;
   QAction      *mpMenuDelete;
   QModelIndex  mContextModelIndex;
   QFileInfo    mFileInfo;
};

#endif
