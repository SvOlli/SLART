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

class QGridLayout;
class QListWidget;
class QPushButton;


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
   
private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );
   
   ProxyWidget  *mpProxyWidget;
   QListWidget  *mpLogList;
};

/* defaults */
#define VALUE_BYTES               value( "Bytes", 0 ).toULongLong()
#define VALUE_DIRECTORY           value("Directory", QDir::currentPath()).toString()
#define VALUE_FILES               value( "Files", 0 ).toUInt()
#define VALUE_SLARTCOMMUNICATION  value("SLARTCommunication", false).toBool()

#endif
