/**
 * src/apps/Funkytown/ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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
class QLabel;
class QListWidget;
class QPushButton;
class QTextBrowser;

/* forward declaration of local classes */
class GlobalConfigWidget;
class ProxyWidget;


class ConfigDialog : public QDialog
{
   Q_OBJECT

public:
   ConfigDialog( QWidget *parent = 0 );

public slots:
   /* execute the dialog */
   void exec();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* send a message to log widget */
   void logMessage( const QString &message );
   /* handle the clear button */
   void handleClear();

private:
   ConfigDialog( const ConfigDialog &that );
   ConfigDialog &operator=( const ConfigDialog &that );

   ProxyWidget        *mpProxyWidget;
   QListWidget        *mpLogList;
   QTextBrowser       *mpHelpText;
   GlobalConfigWidget *mpGlobalConfigWidget;
   QCheckBox          *mpOverwrite;
   QCheckBox          *mpCoverArt;
   QCheckBox          *mpTollKeep;
   QLabel             *mpDownloadedFiles;
   QLabel             *mpDownloadedBytes;
   QPushButton        *mpClearButton;
};

/* defaults */
#define VALUE_BYTES               value("Bytes", 0).toULongLong()
#define VALUE_COVERART            value("CoverArt", false).toBool()
#define VALUE_DIRECTORY           value("Directory", QDir::currentPath()).toString()
#define VALUE_FILES               value("Files", 0).toUInt()
#define VALUE_OVERWRITE           value("Overwrite", false).toBool()
#define VALUE_SLARTCOMMUNICATION  value("SLARTCommunication", false).toBool()
#define VALUE_STYLESHEET          value("StyleSheet", QString()).toString()
#define VALUE_TOLLKEEP            value("TollKeep", true).toBool()
#define VALUE_USERAGENT           value("UserAgent", QString("Shockwave Flash")).toString()

#endif
