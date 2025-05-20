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
   InnuendoConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );

public slots:
   /*!
    \brief execute the dialog

   */
   int exec();
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
   Q_DISABLE_COPY( InnuendoConfigDialog )

   SatelliteConfigWidget   *mpSatelliteConfigWidget; /*!< \brief \todo TODO */
   GlobalConfigWidget      *mpGlobalConfigWidget; /*!< \brief \todo TODO */
   ProxyWidget             *mpProxyWidget; /*!< \brief \todo TODO */
   QSpinBox                *mpBufferSize; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartPartyman; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartKarmadrome; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartRubberbandman; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartStripped; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartFunkytown; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartNotorious; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartCreep; /*!< \brief \todo TODO */
   QPushButton             *mpAutostartUnderpass; /*!< \brief \todo TODO */
};

/*! @} */

#endif
