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
class GenericSatMsgHandler;
class Satellite;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* handle eject button */
   void eject();
   /* update configuration changes */
   void handleConfigUpdate();
   /* set buttons active according to state "no drive found" */
   void stateNoDrive();
   /* set buttons active according to state "no disc inserted" */
   void stateNoDisc();
   /* set buttons active according to state "disc read" */
   void stateDisc();
   /* set buttons active according to state "scanning the disc" */
   void stateScan();
   /* set buttons active according to state "connecting to freedb" */
   void stateNet();
   /* set buttons active according to state "ripping tracks" */
   void stateRip();
   
signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
public:
private:
   Satellite            *mpSatellite;
   GenericSatMsgHandler *mpGenericSatMsgHandler;
   CDInfo               *mpCDInfo;
   CDDBClient           *mpCDDBClient;
   CDEdit               *mpCDEdit;
   CDReader             *mpCDReader;
   QLabel               *mpMessage;
   ConfigDialog         *mpConfigDialog;
   QHBoxLayout          *mpButtonLayout;
   QPushButton          *mpSettingsButton;
   QPushButton          *mpCancelButton;
   QPushButton          *mpScanButton;
   QPushButton          *mpRipButton;
   QPushButton          *mpEjectButton;
};

#endif
