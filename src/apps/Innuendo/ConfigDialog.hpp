/*
 * src/apps/Innuendo/ConfigDialog.hpp
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
class ConfigSatelliteWidget;
class GlobalConfigWidget;
class ProxyWidget;
class SatelliteConfigWidget;


/*!
  \addtogroup Innuendo

  @{
*/

/*!
 \brief TODO

 \dotfile "graphs/apps/Innuendo/ConfigDialog_connect.dot" "Connect Graph"
*/
class ConfigDialog : public QDialog
{
   Q_OBJECT

public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

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
   ConfigDialog( const ConfigDialog &that );
   ConfigDialog &operator=( const ConfigDialog &that );

   SatelliteConfigWidget   *mpSatelliteConfigWidget;
   GlobalConfigWidget      *mpGlobalConfigWidget;
   ProxyWidget             *mpProxyWidget;
   QSpinBox                *mpBufferSize;
   QPushButton             *mpAutostartPartyman;
   QPushButton             *mpAutostartKarmadrome;
   QPushButton             *mpAutostartRubberbandman;
   QPushButton             *mpAutostartStripped;
   QPushButton             *mpAutostartFunkytown;
   QPushButton             *mpAutostartNotorious;
   QPushButton             *mpAutostartCreep;
};

/*! @} */

/* defaults */
#define VALUE_BUFFERSIZE         value( "BufferSize", 500 ).toInt()
#define VALUE_SLARTCOMMUNICATION value( "SLARTCommunication", false ).toBool()
#define VALUE_STARTUP            value( "Startup", QStringList() ).toStringList()
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()
#endif
