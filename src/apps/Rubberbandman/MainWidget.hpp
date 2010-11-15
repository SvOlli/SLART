/**
 * src/apps/Rubberbandman/MainWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QTabWidget;
class QPushButton;

/* forward declaration of local classes */
class BrowseWidget;
class ConfigDialog;
class Database;
class DatabaseWidget;
class SatelliteWidget;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();
   
public slots:
   /* switch to filesystem tab */
   void goToFilesystem();
   /* save current tab for restart */
   void handleTabChange( int tabNr );
   
signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &that );
   MainWidget &operator=( const MainWidget &that );
   
   Database          *mpDatabase;
   BrowseWidget      *mpBrowseWidget;
   SatelliteWidget   *mpSatelliteWidget;
   DatabaseWidget    *mpDatabaseWidget;
   QTabWidget        *mpTabs;
   QPushButton       *mpSettingsButton;
   ConfigDialog      *mpConfigDialog;
};

#endif
