/**
 * src/apps/Sorcerer/SorcererWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SORCERERWIDGET_HPP
#define SORCERERWIDGET_HPP SORCERERWIDGET_HPP

#include <QWidget>

class QIcon;
class QLabel;
class QPushButton;
class QString;
class QTabWidget;

class Database;
class DatabaseWidget;
class ProxyWidget;
class SatelliteConfigWidget;


class SorcererWidget : public QWidget
{
Q_OBJECT
   
public:
   SorcererWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~SorcererWidget();
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
   SorcererWidget( const SorcererWidget &other );
   SorcererWidget &operator=( const SorcererWidget &other );
   
   Database                *mpDatabase;
   QTabWidget              *mpTabs;
   QLabel                  *mpHint;
   QPushButton             *mpNext;
   
   DatabaseWidget          *mpDatabaseWidget;
   SatelliteConfigWidget   *mpSatelliteConfigWidget;
   ProxyWidget             *mpProxyWidget;
   
   int                     mLastTab;
   bool                    mDatabaseOk;
   bool                    mCommunicationOk;
   bool                    mProxyOk;
};

#endif
