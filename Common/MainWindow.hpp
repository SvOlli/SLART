/**
 * MainWindow.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

#include "MainWidget.hpp"
#ifndef MAINWINDOW_SORCERER
#warning MAINWINDOW_SORCERER not defined
#endif
#ifndef MAINWINDOW_CHANGETITLE
#warning MAINWINDOW_CHANGETITLE not defined
#endif
#ifndef MAINWINDOW_PROHIBITCLOSE
#warning MAINWINDOW_PROHIBITCLOSE not defined
#endif


#include <QMainWindow>

#include <QPoint>

class QApplication;
class QDir;


class MainWindow : public QMainWindow
{
Q_OBJECT
   
public:
   MainWindow( bool saveWindow = true, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
   /* passing through the only widget of the window */
   MainWidget *mainWidget();
#if MAINWINDOW_SORCERER
   /* call Sorcerer for setting up */
   static bool invokeSetUp( QApplication *app );
#endif
   
public slots:
#if MAINWINDOW_CHANGETITLE
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );
#endif
#if MAINWINDOW_PROHIBITCLOSE
   /* prohibit closing of window (Partyman kiosk mode) */
   void prohibitClose( bool prohibit ) { mProhibitCloseWindow = prohibit; };
#endif

protected:
   /* intercept for writing the settings */
   virtual void closeEvent( QCloseEvent *event );
   /* very ugly workaround for wrong position restoration on Ubuntu */
   virtual bool event( QEvent *event );
   
private:
   MainWindow( const MainWindow &other );
   MainWindow &operator=( const MainWindow &other );
   
#if MAINWINDOW_SORCERER
   /* try to load and run Sorcerer */
   static bool trySorcerer( QApplication *app, const QDir &dir );
#endif
   
#if MAINWINDOW_PROHIBITCLOSE
   bool          mProhibitCloseWindow;
#endif
   bool          mSaveWindow;
   int           mForbidMove;
   MainWidget   *mpMainWidget;
};

#endif
