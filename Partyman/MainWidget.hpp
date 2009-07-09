/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      1
#define MAINWINDOW_CHANGETITLE   1
#define MAINWINDOW_PROHIBITCLOSE 1

#include <QWidget>

class QPushButton;

class ConfigDialog;
class ControlWidget;
class Database;
class PlaylistWidget;

class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   ~MainWidget();
   
   /* prepare startup of application */
   void startUp();
   
protected:
   
public slots:
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );
   /* don't autostart if database is empty */
   void allowAutostart( bool allow );
   
signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   void kioskMode( bool prohibit );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   bool            mAllowAutostart;
   QWidget         *mpParent;
   Database        *mpDatabase;
   ConfigDialog    *mpConfig;
   PlaylistWidget  *mpPlaylist;
   ControlWidget   *mpControl;
};

#endif
