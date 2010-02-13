/**
 * src/apps/Innuendo/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      1
#define MAINWINDOW_CHANGETITLE   0
#define MAINWINDOW_PROHIBITCLOSE 0
#define MAINWIDGET_SCREENSHOT    1

#include <QWidget>

#include <QStringList>

class QKeyEvent;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QIcon;
class QPushButton;
class QSpinBox;
class QStringList;
   
class ConfigDialog;
class DropDialog;
class ExecButton;
class Satellite;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();

public slots:
   /* re-read config */
   void readConfig();
   /* handle the ping button */
   void handlePingButton();
   /* handle SLART message */
   void handleSatellite( const QByteArray &message );
   /* copy clicked line of message buffer to clipboard */
   void listWidgetItemToClipboard( QListWidgetItem *item );

private slots:
   /* autostart */
   void autostart();

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
protected:
   /* for implementing dropping */
   virtual void dragEnterEvent( QDragEnterEvent *event );
   /* for implementing dropping */
   virtual void dropEvent( QDropEvent *event );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   Satellite    *mpSatellite;
   QListWidget  *mpMessageBuffer;
   QPushButton  *mpSettingsButton;
   QPushButton  *mpPingButton;
   ConfigDialog *mpConfig;
   DropDialog   *mpDropDialog;
   ExecButton   **mpExecButtons;
   int          mNumExecButtons;
   QStringList  mAutostart;
};

#endif
