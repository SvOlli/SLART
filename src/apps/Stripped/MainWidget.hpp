/**
 * src/apps/Stripped/MainWidget.hpp
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
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QLabel;
class QPushButton;
class QHBoxLayout;

/* forward declaration of local classes */
class CDDBClient;
class CDEdit;
class CDInfo;
class CDList;
class CDReader;
class ConfigDialog;
class Satellite;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* set the directory where the ripped tracks go to */
   void setRippingDir();
   /* handle eject button */
   void eject();
   /*  */
   void showMessage( const QString &message );
   /* update configuration changes */
   void handleConfigUpdate();
   /*  */
   void stateNoDrive();
   /*  */
   void stateNoDisc();
   /*  */
   void stateDisc();
   /*  */
   void stateScan();
   /*  */
   void stateNet();
   /*  */
   void stateRip();
   
signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
public:
private:
   Satellite       *mpSatellite;
   QPushButton     *mpDirButton;
   CDInfo          *mpCDInfo;
   CDDBClient      *mpCDDBClient;
   CDEdit          *mpCDEdit;
   CDReader        *mpCDReader;
   QLabel          *mpMessage;
   ConfigDialog    *mpConfigDialog;
   QHBoxLayout     *mpButtonLayout;
   QPushButton     *mpSettingsButton;
   QPushButton     *mpCancelButton;
   QPushButton     *mpScanButton;
   QPushButton     *mpRipButton;
   QPushButton     *mpEjectButton;
};

#endif
