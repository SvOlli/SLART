/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      0
#define MAINWINDOW_CHANGETITLE   0
#define MAINWINDOW_PROHIBITCLOSE 0

#include <QWidget>

class QLineEdit;

class ConfigDialog;
class FreeDB;
class Foundlist;
class SearchWidget;

class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

protected:
   
public slots:
   void runQuery( const QString &query );
   void handleClicked( const QString &category, const QString &id );
   
signals:
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   FreeDB       *mpFreeDB;
   SearchWidget *mpSearch;
   Foundlist    *mpFound;
   Foundlist    *mpCDInfo;
   ConfigDialog *mpConfig;
};

#endif
