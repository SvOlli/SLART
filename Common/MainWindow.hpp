/**
 * MainWindow.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

#include <QMainWindow>


class MainWidget;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
   MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /* passing through the only widget of the window */
   MainWidget *mainWidget();
   /* intercept for writing the settings */
   void closeEvent( QCloseEvent *event );

public slots:
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );

private:
   MainWindow( const MainWindow &other );
   MainWindow &operator=( const MainWindow &other );
   
   MainWidget   *mpMainWidget;
};

#endif
