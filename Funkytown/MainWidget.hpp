/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>

class DownloadHandler;
class PostDownloadHandler;
class PostDownloadHandlerHTML;
class PostDownloadHandlerXML;
class PostDownloadHandlerMP3;
class PostDownloadHandlerFLV;

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
   QPushButton             *mpSetupButton;
   ConfigDialog            *mpConfigDialog;

};

#endif
