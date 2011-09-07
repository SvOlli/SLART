/**
 * src/apps/Rubberbandman/FileSysBrowser.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FILESYSBROWSER_HPP
#define FILESYSBROWSER_HPP FILESYSBROWSER_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QModelIndex>
#include <QFileInfo>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QDirModel;
class QLineEdit;
class QPushButton;
class QModelIndex;
class QShowEvent;
class QTreeView;

/* forward declaration of local classes */
class DatabaseInterface;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class FileSysBrowser : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   FileSysBrowser( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~FileSysBrowser();

public slots:
   /*!
    \brief get the index of clicked entry

   */
   void entryClicked( const QModelIndex &index );
   /*!
    \brief change the root dir

   */
   void handleRootDir();
   /*!
    \brief handle dir up button

   */
   void handleDotButton();
   /*!
    \brief show a filename in browser

   */
   void scrollTo( const QString &fileName );
   /*!
    \brief handle right mouse button (send track name to Partyman)

   */
   void contextMenu( const QPoint &pos );
   /*!
    \brief handle menu entry "Send To Partyman"

   */
   void menuSendToPartyman();
   /*!
    \brief handle menu entries "Move" and "Move Directory"

   */
   void menuMove( bool withContent = false );
   /*!
    \brief handle menu entry "Move Content" with wrapper for menuMove()

   */
   void menuMoveContent();
   /*!
    \brief handle menu entry "Rename"

   */
   void menuRename();
   /*!
    \brief handle menu entry "Delete"

   */
   void menuDelete();

signals:
   /*!
    \brief emit the path of clicked entry

   */
   void clicked( const QString &path );

protected:
   /*!
    \brief reimplemented for the drop of drag'n'drop of tracks

   */
   virtual void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief reimplemented for the drop of drag'n'drop of tracks

   */
   virtual void dropEvent( QDropEvent *event );

private:
   Q_DISABLE_COPY( FileSysBrowser )

   DatabaseInterface    *mpDatabase;
   QLineEdit            *mpRootDir;
   QPushButton          *mpDotButton;
   QTreeView            *mpView;
   QDirModel            *mpModel;
   QAction              *mpMenuSendToPartyman;
   QAction              *mpMenuRescan;
   QAction              *mpMenuMoveFile;
   QAction              *mpMenuMoveDirectory;
   QAction              *mpMenuMoveContent;
   QAction              *mpMenuRename;
   QAction              *mpMenuDelete;
   QModelIndex          mContextModelIndex;
   QFileInfo            mFileInfo;
};

/*! @} */

#endif
