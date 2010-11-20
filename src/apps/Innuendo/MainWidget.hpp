/**
 * src/apps/Innuendo/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */
/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QKeyEvent;
class QLabel;
class QListWidgetItem;
class QIcon;
class QPushButton;
class QSpinBox;
class QStringList;

/* forward declaration of local classes */
class ConfigDialog;
class DropDialog;
class ExecButton;
class GenericSatMsgHandler;
class LogListWidget;
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
   void handleSatellite( const QByteArray &msg );
   /* copy clicked line of message buffer to clipboard */
   void listWidgetItemToClipboard( QListWidgetItem *item );

private slots:
   /* autostart */
   void autostart();
   /* prohibit autostart, in case Innuendo is already running */
   void noAutostart();

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );

protected:
   /* for implementing dropping */
   virtual void dragEnterEvent( QDragEnterEvent *event );
   /* for implementing dropping */
   virtual void dropEvent( QDropEvent *event );

private:
   MainWidget( const MainWidget &that );
   MainWidget &operator=( const MainWidget &that );

   Satellite            *mpSatellite;
   GenericSatMsgHandler *mpGenericSatMsgHandler;
   LogListWidget        *mpMessageBuffer;
   QPushButton          *mpSettingsButton;
   QPushButton          *mpPingButton;
   ConfigDialog         *mpConfig;
   DropDialog           *mpDropDialog;
   ExecButton           **mpExecButtons;
   int                  mNumExecButtons;
   QStringList          mAutostart;
};

#endif
