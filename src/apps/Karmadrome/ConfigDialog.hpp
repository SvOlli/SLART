/**
 * src/apps/Karmadrome/ConfigDialog.hpp
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
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

/* forward declaration of local classes */
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
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()

#endif
