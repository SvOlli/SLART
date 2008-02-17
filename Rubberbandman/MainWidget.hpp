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
class SLATComWidget;

class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   bool shutdown();

public slots:
   
signals:
   void requestChangeTitle(const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   BrowseWidget   *mpBrowseWidget;
   SLATComWidget  *mpSLATComWidget;
   QTabWidget     *mpTabs;
};

#endif
