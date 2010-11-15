/**
 * src/libs/Sorcerer/SorcererWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERWIDGET_HPP
#define SORCERERWIDGET_HPP SORCERERWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QIcon;
class QLabel;
class QPushButton;
class QTabWidget;

/* forward declaration of local classes */
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
   SorcererWidget( const SorcererWidget &that );
   SorcererWidget &operator=( const SorcererWidget &that );
   
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
   const QString           mQuit;
   const QString           mStart;
};

#endif
