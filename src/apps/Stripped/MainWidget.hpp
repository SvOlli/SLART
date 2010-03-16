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
   enum operationState
   {
      stateInit,
      stateNoDrive,
      stateIdle,
      stateScan,
      stateNet,
      stateRip
   };

   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
   /*  */
   void setState( enum operationState state );

public slots:
   /* set the directory where the ripped tracks go to */
   void setRippingDir();
   /* handle eject button */
   void eject();
   /* handle working state (disable buttons) */
   void working( bool allowCancel = true );
   /* unset stuff done by settings to working state */
   void finished();
   /*  */
   void showMessage( const QString &message );
   /* update configuration changes */
   void handleConfigUpdate();
   
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
   enum operationState  mState;
};

#endif
