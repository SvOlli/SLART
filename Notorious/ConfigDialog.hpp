/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>

#include "ProxyWidget.hpp"

class QCheckBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTextBrowser;

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
   /*  */
   void handleProgress( unsigned count, const char *filename );

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );
   
   FreeDBImport *mpFreeDBImport;
   QListWidget  *mpLogList;
   QLineEdit    *mpImportFile;
   QLabel       *mpCount;
   QLabel       *mpFileName;
   QPushButton  *mpImportButton;
};

/* defaults */
#define VALUE_BYTES              value( "Bytes", 0 ).toULongLong()
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()

#endif
