/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

class ConfigNotifyWidget;
class GlobalConfigWidget;
class ProxyWidget;


class ConfigDialog : public QDialog
{
Q_OBJECT
   
public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   /* execute the dialog */
   void exec();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   
signals:
   /* configuration has changed */
   void configChanged();
   
private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );
   
   GlobalConfigWidget   *mpGlobalConfigWidget;
   QLabel               *mpNumColumnsLabel;
   QSpinBox             *mpNumColumns;
   QCheckBox            *mpClearBeforeImport;
   QCheckBox            *mpExportAsRelative;
   QCheckBox            *mpRandomizeExport;
};

/* defaults */
#define VALUE_CLEARBEFOREIMPORT  value( "ClearBeforeImport", false ).toBool()
#define VALUE_EXPORTASRELATIVE   value( "ExportAsRelative", false ).toBool()
#define VALUE_EXPORTDIRECTORY    value( "ExportDirectory", QString() ).toString()
#define VALUE_IMPORTDIRECTORY    value( "ImportDirectory", QString() ).toString()
#define VALUE_NUMBEROFCOLUMNS    value( "NumberOfColumns", 3 ).toInt()
#define VALUE_RANDOMIZEEXPORT    value( "RandomizeExport", false ).toBool()

#endif
