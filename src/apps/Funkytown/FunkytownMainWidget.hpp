/**
 * src/apps/Funkytown/FunkytownMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FUNKYTOWNMAINWIDGET_HPP
#define FUNKYTOWNMAINWIDGET_HPP FUNKYTOWNMAINWIDGET_HPP

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
class FunkytownMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   FunkytownMainWidget( QWidget *parent = 0 );
   /*!
    \brief destructor

    */
   virtual ~FunkytownMainWidget();

public slots:
   /*!
    \brief open download dir requester

   */
   void setDownloadDir();

   /*!
    \brief enqueue a download

    \param name
   */
   void downloadUserPage( const QString &name = QString() );

   /*!
    \brief handle if download is active

    \param downloadActive
   */
   void downloadActive( bool downloadActive = false );

signals:
   /*!
    \brief request to change the title and icon

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

protected:
   /*!
    \brief for implementing dropping

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );

   /*!
    \brief for implementing dropping

    \param event
   */
   void dropEvent( QDropEvent *event );

private:
   Q_DISABLE_COPY( FunkytownMainWidget )

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
