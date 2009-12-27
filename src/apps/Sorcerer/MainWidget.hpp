/**
 * src/apps/Sorcerer/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      0
#define MAINWINDOW_CHANGETITLE   1
#define MAINWINDOW_PROHIBITCLOSE 0
#define MAINWIDGET_SCREENSHOT    0

#include <QWidget>

class QIcon;
class QLabel;
class QPushButton;
class QString;
class QTabWidget;

class ConfigNotifyWidget;
class Database;
class DatabaseWidget;
class ProxyWidget;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();
   /* bitmask of errors occured */
   int errors();
   
public slots:
   /* handle the change of a tab */
   void handleTabChange( int newTab );
   /* handle the next button */
   void handleNextButton();
   /* unlock the option to leave database tab when everything is ok */
   void unlockDatabase();
   /* unlock the option to leave communication tab */
   void unlockCommunication();
   
signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   Database             *mpDatabase;
   QTabWidget           *mpTabs;
   QLabel               *mpHint;
   QPushButton          *mpNext;
   
   DatabaseWidget       *mpDatabaseWidget;
   ConfigNotifyWidget   *mpConfigCommunicationWidget;
   ProxyWidget          *mpProxyWidget;
   
   int                  mLastTab;
   bool                 mDatabaseOk;
   bool                 mCommunicationOk;
   bool                 mProxyOk;
};

#endif
