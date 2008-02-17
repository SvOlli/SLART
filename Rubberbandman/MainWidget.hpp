/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>

class QTabWidget;
class BrowseWidget;

class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   bool shutdown();

public slots:

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   BrowseWidget   *mpBrowseWidget;
   QTabWidget     *mpTabs;
};

#endif
