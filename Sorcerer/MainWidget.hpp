/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

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
