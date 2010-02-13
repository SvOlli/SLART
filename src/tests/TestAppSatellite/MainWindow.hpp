/**
 * MainWindow.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP


#include <QMainWindow>

class QApplication;
class QDir;


class MainWindow : public QMainWindow
{
Q_OBJECT
   
public:
   MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
private:
   MainWindow( const MainWindow &other );
   MainWindow &operator=( const MainWindow &other );
};

#endif
