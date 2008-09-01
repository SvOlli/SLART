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
class QString;
class QTabWidget;
class QLabel;
class QPushButton;

class ConfigNotifyWidget;
class Database;
class DatabaseWidget;
class ProxyWidget;

class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   ~MainWidget();
   int errors();

public slots:
   void handleTabChange( int newTab );
   void handleNextButton();
   void unlockDatabase();
   void unlockCommunication();

signals:
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
