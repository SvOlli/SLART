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
class SLARTComWidget;
class DatabaseWidget;
class QPushButton;
class ConfigDialog;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   bool shutdown();

public slots:
   /* switch to filesystem tab */
   void goToFilesystem();
   /* save current tab for restart */
   void handleTabChange( int tabNr );

signals:
   void requestChangeTitle(const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   BrowseWidget   *mpBrowseWidget;
   SLARTComWidget *mpSLARTComWidget;
   DatabaseWidget *mpDatabaseWidget;
   QTabWidget     *mpTabs;
   QPushButton    *mpSettingsButton;
   ConfigDialog   *mpConfigDialog;
};

#endif
