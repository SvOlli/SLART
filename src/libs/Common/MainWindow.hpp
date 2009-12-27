/**
 * MainWindow.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

#include <QMainWindow>

#include <QPoint>

class QApplication;
class QDir;

class MainWindow : public QMainWindow
{
Q_OBJECT
   
public:
   MainWindow( bool saveWindow = true,
               QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*  */
   void setMainWidget( QWidget *mainWidget );
   /*  */
   void enableScreenshot();

public slots:
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );
   /* prohibit closing of window (Partyman kiosk mode) */
   void prohibitClose( bool prohibit ) { mProhibitCloseWindow = prohibit; };

protected:
   /* intercept for writing the settings */
   virtual void closeEvent( QCloseEvent *event );
   /* very ugly workaround for wrong position restoration on Ubuntu */
   virtual bool event( QEvent *event );
   
private:
   MainWindow( const MainWindow &other );
   MainWindow &operator=( const MainWindow &other );
   
   bool          mProhibitCloseWindow;
   bool          mSaveWindow;
   int           mForbidMove;
   QWidget       *mpMainWidget;
};

#endif
