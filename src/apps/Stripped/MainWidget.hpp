/**
 * src/apps/Stripped/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      0
#define MAINWINDOW_CHANGETITLE   0
#define MAINWINDOW_PROHIBITCLOSE 0
#define MAINWIDGET_SCREENSHOT    0

#include <QWidget>

#include <QList>

class QComboBox;
class QPushButton;
class QHBoxLayout;

class CDReader;
class CDToc;
class CDDB;
class CDEdit;
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
   /* handle working state (disable buttons) */
   void working( bool allowCancel = true );
   /* unset stuff done by settings to working state */
   void finished();
   
signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   Satellite       *mpSatellite;
   QPushButton     *mpDirButton;
   CDToc           *mpToc;
   CDDB            *mpCDDB;
   CDEdit          *mpCDEdit;
   CDReader        *mpCDReader;
   ConfigDialog    *mpConfigDialog;
   QPushButton     *mpSettingsButton;
   QPushButton     *mpCancelButton;
   QHBoxLayout     *mpButtonLayout;
   QPushButton     *mpTocButton;
   QPushButton     *mpCDTextButton;
   QPushButton     *mpRipButton;
   QPushButton     *mpEjectButton;
};

#endif
