/*
 * src/apps/Innuendo/InnuendoConfigDialog.hpp
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
 \brief \todo complete documentation

 \dotfile "graphs/apps/Innuendo/InnuendoConfigDialog_connect.dot" "Connect Graph"
*/
class InnuendoConfigDialog : public QDialog
{
   Q_OBJECT

public:
   InnuendoConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

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
   InnuendoConfigDialog( const InnuendoConfigDialog &that );
   InnuendoConfigDialog &operator=( const InnuendoConfigDialog &that );

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

#endif
