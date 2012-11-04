/*
 * src/apps/Karmadrome/KarmadromeConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef KARMADROMECONFIGDIALOG_HPP
#define KARMADROMECONFIGDIALOG_HPP KARMADROMECONFIGDIALOG_HPP

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
class GlobalConfigWidget;
class ProxyWidget;


/*!
  \addtogroup Karmadrome

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Karmadrome/KarmadromeConfigDialog_connect.dot" "Connect Graph"
*/
class KarmadromeConfigDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
    \param flags
   */
   KarmadromeConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief destructor

   */
   virtual ~KarmadromeConfigDialog();

public slots:
   /*!
    \brief execute the dialog

   */
   void exec();
   /*!
    \brief read settings from storage

   */
   void readSettings();
   /*!
    \brief write settings to storage

   */
   void writeSettings();

signals:
   /*!
    \brief configuration has changed

   */
   void configChanged();

private:
   KarmadromeConfigDialog( const KarmadromeConfigDialog &that );
   KarmadromeConfigDialog &operator=( const KarmadromeConfigDialog &that );

   GlobalConfigWidget   *mpGlobalConfigWidget;
   QLabel               *mpNumColumnsLabel;
   QSpinBox             *mpNumColumns;
   QCheckBox            *mpClearBeforeImport;
   QCheckBox            *mpExportAsRelative;
   QCheckBox            *mpRandomizeExport;
   QCheckBox            *mpUseCheckBoxes;
};

/*! @} */

/* defaults */
#define VALUE_CLEARBEFOREIMPORT  value( "ClearBeforeImport", false ).toBool()
#define VALUE_EXPORTASRELATIVE   value( "ExportAsRelative", false ).toBool()
#define VALUE_EXPORTDIRECTORY    value( "ExportDirectory", QString() ).toString()
#define VALUE_IMPORTDIRECTORY    value( "ImportDirectory", QString() ).toString()
#define VALUE_NUMBEROFCOLUMNS    value( "NumberOfColumns", 3 ).toInt()
#define VALUE_RANDOMIZEEXPORT    value( "RandomizeExport", false ).toBool()
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()
#define VALUE_USECHECKBOXES      value( "UseCheckBoxes", false ).toBool()

#endif
