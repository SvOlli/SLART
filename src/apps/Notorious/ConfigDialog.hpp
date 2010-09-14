/**
 * src/apps/Notorious/ConfigDialog.hpp
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
#include <QTime>

/* local library headers */
#include <ProxyWidget.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTextBrowser;

/* forward declaration of local classes */
class FreeDBImport;


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
   /* run or stop an import job */
   void handleImport();
   /* pop up requester for selecting base dir */
   void setFileName();
   /* update buttons to show if base dir is valid */
   void checkValidFile( const QString &fileName );
   /* handle the progress of an import */
   void handleProgress( unsigned count, const char *filename );

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );
   
   FreeDBImport *mpFreeDBImport;
   QListWidget  *mpLogList;
   QLineEdit    *mpImportFile;
   QLabel       *mpCount;
   QLabel       *mpFileName;
   QLabel       *mpTimeSpent;
   QPushButton  *mpImportButton;
   QTime        mTimeSpent;
};

/* defaults */
#define VALUE_BYTES              value( "Bytes", 0 ).toULongLong()
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()

#endif
