/**
 * src/apps/Partyman/ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

/* forward declaration of local classes */
class Database;
class GlobalConfigWidget;
class MyClipboard;


class ConfigDialog : public QDialog
{
Q_OBJECT

public:
   ConfigDialog( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   /* execute the dialog */
   void exec();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* handle "start DerMixD" checkbox */
   void handleDerMixDrun( bool checked );
   /* handle "Nomalize" combobox */
   void handleNormalizeMode( int mode );
   /* handle "Show Tray Icon" checkbox */
   void handleShowTrayIcon( bool checked );
   /* activate kiosk mode */
   void handleStartKiosk();

signals:
   /* configuration has changed */
   void configChanged();
   /* update of browser tree requested */
   void updateBrowser();

private:
   ConfigDialog( const ConfigDialog &that );
   ConfigDialog &operator=( const ConfigDialog &that );


   Database             *mpDatabase;
   QLabel               *mpDerMixDhostLabel;
   QLineEdit            *mpDerMixDhost;
   QLabel               *mpDerMixDportLabel;
   QSpinBox             *mpDerMixDport;
   QCheckBox            *mpDerMixDlog;
   QCheckBox            *mpDerMixDrun;
   QLabel               *mpDerMixDcmdLabel;
   QLineEdit            *mpDerMixDcmd;
   QLabel               *mpDerMixDparamsLabel;
   QLineEdit            *mpDerMixDparams;
   QCheckBox            *mpAutoConnect;
   QSpinBox             *mpCrossfadeTime;
   QComboBox            *mpNormalizeMode;
   QDoubleSpinBox       *mpNormalizeValue;
   MyClipboard          *mpClipboard;
   QLineEdit            *mpLogCmd;
   QCheckBox            *mpCountSkip;
   QCheckBox            *mpTrayIcon;
   QCheckBox            *mpTrayIconBubble;
   QDoubleSpinBox       *mpTrayIconBubbleTime;
   QCheckBox            *mpPlayOnlyFavorite;
   QCheckBox            *mpPlayOnlyLeastPlayed;
   QComboBox            *mpPlayFolder;
   QSpinBox             *mpPlayNotAgainCount;
   QLineEdit            *mpNamePattern;
   QLineEdit            *mpPlayerPattern;
   QLineEdit            *mpListPattern;
   QLineEdit            *mpTrayIconPattern;
   QCheckBox            *mpSplitterVertical;
   QPushButton          *mpUpdateBrowserButton;
   QPushButton          *mpStartKioskButton;
   GlobalConfigWidget   *mpGlobalSettings;
};

/* defaults */
#define VALUE_AUTOCONNECT         value("AutoConnect", false).toBool()
#define VALUE_COUNTSKIP           value("CountSkip", false).toBool()
#define VALUE_CROSSFADETIME       value("CrossfadeTime", 10).toInt()
#define VALUE_DERMIXDCMD          value("DerMixDcmd", "dermixd").toString()
#define VALUE_DERMIXDHOST         value("DerMixDhost", "localhost").toString()
#define VALUE_DERMIXDLOG          value("DerMixDlog", false).toBool()
#define VALUE_DERMIXDPARAMS       value("DerMixDparams", "").toString()
#define VALUE_DERMIXDPORT         value("DerMixDport", 8888).toInt()
#define VALUE_DERMIXDRUN          value("DerMixDrun", true).toBool()
#define VALUE_LOGCMD              value("LogCmd", "").toString()
#define VALUE_LISTPATTERN         value("ListPattern", "(|$PLAYTIME|)|$ARTIST| - |$TITLE|").toString()
#define VALUE_NAMEPATTERN         value("NamePattern", QApplication::applicationName()+": |$TITLE|").toString()
#define VALUE_NORMALIZEMODE       value("NormalizeMode", 0).toInt()
#define VALUE_NORMALIZEVALUE      value("NormalizeValue", 0.4).toDouble()
#define VALUE_PLAYERPATTERN       value("PlayerPattern", "|$ARTIST| - |$TITLE|").toString()
#define VALUE_PLAYFOLDER          value("PlayFolder").toString()
#define VALUE_PLAYNOTAGAINCOUNT   value("PlayNotAgainCount", 10).toInt()
#define VALUE_PLAYONLYFAVORITE    value("PlayOnlyFavorite", false).toBool()
#define VALUE_PLAYONLYLEASTPLAYED value("PlayOnlyLeastPlayed", false).toBool()
#define VALUE_RANDOMTRIES         value("RandomTries", 10).toInt()
#define VALUE_SPLITTERVERTICAL    value("SplitterVertical", false).toBool()
#define VALUE_STYLESHEET          value("StyleSheet", QString()).toString()
#define VALUE_TRAYICON            value("TrayIcon", false).toBool()
#define VALUE_TRAYICONBUBBLE      value("TrayIconBubble", true).toBool()
#define VALUE_TRAYICONBUBBLEICON  value("TrayIconBubbleIcon", 0).toInt()
#define VALUE_TRAYICONBUBBLETIME  value("TrayIconBubbleTime", 4.0).toDouble()
#define VALUE_TRAYICONPATTERN     value("TrayIconPattern", "|$ARTIST|#n|$TITLE|#n|$ALBUM|").toString()

#endif
