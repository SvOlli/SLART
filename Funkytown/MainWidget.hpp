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

class QDropEvent;
class QFile;
class QGridLayout;
class QHttp;
class QHttpResponseHeader;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class SetupDialog;

class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0 );
   virtual ~MainWidget();

public slots:
   void setDownloadDir();
   void downloadUserPage( const QString &name = QString() );
   void showSetupDialog();
   void downloadActive( bool downloadActive = false );
#if 0
   void gotCloseSignal();
#endif

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );

protected:
   void dragEnterEvent( QDragEnterEvent *event );
   void dropEvent( QDropEvent *event );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   void resetProgress( bool isRunning );

   QGridLayout             *mpLayout;
   QLabel                  *mpDirText;
   QPushButton             *mpDirButton;
   QLabel                  *mpNameText;
   QLineEdit               *mpNameInput;
   QPushButton             *mpGoButton;
   DownloadHandler         *mpDownloadHandler;
   QPushButton             *mpSetupButton;
   PostDownloadHandler     *mpPostDownloadHandler;
   PostDownloadHandlerHTML *mpPostDownloadHandlerHTML;
   PostDownloadHandlerXML  *mpPostDownloadHandlerXML;
   PostDownloadHandlerMP3  *mpPostDownloadHandlerMP3;
   SetupDialog             *mpSetupDialog;

};

#endif
