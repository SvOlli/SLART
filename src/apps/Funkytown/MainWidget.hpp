/**
 * src/apps/Funkytown/MainWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      0
#define MAINWINDOW_CHANGETITLE   0
#define MAINWINDOW_PROHIBITCLOSE 0
#define MAINWIDGET_SCREENSHOT    0

#include <QWidget>

class QDropEvent;
class QFile;
class QGridLayout;
class QHttp;
class QHttpResponseHeader;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;

class ConfigDialog;
class DownloadHandler;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0 );
   
public slots:
   /* open download dir requester */
   void setDownloadDir();
   /* enqueue a download */
   void downloadUserPage( const QString &name = QString() );
   /* handle if download is active */
   void downloadActive( bool downloadActive = false );
   
signals:
   /* request to change the title and icon */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
protected:
   /* for implementing dropping */
   void dragEnterEvent( QDragEnterEvent *event );
   /* for implementing dropping */
   void dropEvent( QDropEvent *event );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   QGridLayout             *mpLayout;
   QLabel                  *mpDirText;
   QPushButton             *mpDirButton;
   QLabel                  *mpNameText;
   QLineEdit               *mpNameInput;
   QPushButton             *mpGoButton;
   QPushButton             *mpSettingsButton;
   ConfigDialog            *mpConfigDialog;
   DownloadHandler         *mpDownloadHandler;
};

#endif
