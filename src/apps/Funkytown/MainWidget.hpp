/**
 * src/apps/Funkytown/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDropEvent;
class QFile;
class QGridLayout;
class QHttp;
class QHttpResponseHeader;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;

/* forward declaration of local classes */
class ConfigDialog;
class DownloadHandler;

/*!
  \addtogroup Funkytown Funkytown: File Downloader
  @{
  */


/*!
 \brief

*/
class MainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief

    \param parent
   */
   MainWidget( QWidget *parent = 0 );

public slots:
   /* open download dir requester */
   /*!
    \brief

   */
   void setDownloadDir();
   /* enqueue a download */
   /*!
    \brief

    \param name
   */
   void downloadUserPage( const QString &name = QString() );
   /* handle if download is active */
   /*!
    \brief

    \param downloadActive
   */
   void downloadActive( bool downloadActive = false );

signals:
   /* request to change the title and icon */
   /*!
    \brief

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

protected:
   /* for implementing dropping */
   /*!
    \brief

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );
   /* for implementing dropping */
   /*!
    \brief

    \param event
   */
   void dropEvent( QDropEvent *event );

private:
   /*!
    \brief

    \param that
   */
   MainWidget( const MainWidget &that );
   /*!
    \brief

    \param that
   */
   MainWidget &operator=( const MainWidget &that );

   QGridLayout             *mpLayout; /*!< TODO */
   QLabel                  *mpDirText; /*!< TODO */
   QPushButton             *mpDirButton; /*!< TODO */
   QLabel                  *mpNameText; /*!< TODO */
   QLineEdit               *mpNameInput; /*!< TODO */
   QPushButton             *mpGoButton; /*!< TODO */
   QPushButton             *mpSettingsButton; /*!< TODO */
   ConfigDialog            *mpConfigDialog; /*!< TODO */
   DownloadHandler         *mpDownloadHandler; /*!< TODO */
};

/*! @} */

#endif
