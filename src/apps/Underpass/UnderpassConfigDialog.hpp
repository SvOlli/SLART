/*
 * src/apps/Underpass/UnderpassConfigDialog.hpp
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
class QPushButton;
class QSpinBox;

/* forward declaration of local classes */
class GlobalConfigWidget;
class ProxyWidget;
class StationStorage;


/*!
  \addtogroup Underpass

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Underpass/UnderpassConfigDialog_connect.dot" "Connect Graph"
*/
class UnderpassConfigDialog : public QDialog
{
   Q_OBJECT

public:
   UnderpassConfigDialog( StationStorage *storage,
                          QWidget *parent = 0, Qt::WindowFlags flags = 0 );

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

   /*!
    \brief register click on create defaults for use on writeSettings()

   */
   void restoreDefaultsClicked();

signals:
   /*!
    \brief configuration has changed

   */
   void configChanged();

private:
   Q_DISABLE_COPY( UnderpassConfigDialog )

   StationStorage          *mpStorage; /*!< \brief \todo TODO */
   GlobalConfigWidget      *mpGlobalConfigWidget; /*!< \brief \todo TODO */
   ProxyWidget             *mpProxyWidget; /*!< \brief \todo TODO */
   QCheckBox               *mpStopOnPartyman;  /*!< \brief \todo TODO */
   QPushButton             *mpRestoreDefaults; /*!< \brief \todo TODO */
   QSpinBox                *mpBufferSize; /*!< \brief \todo TODO */
   bool                    mRestoreDefaultsClicked; /*!< \brief \todo TODO */
};

/*! @} */

#endif
