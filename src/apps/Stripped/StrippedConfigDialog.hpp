/*
 * src/apps/Stripped/StrippedConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef STRIPPEDCONFIGDIALOG_HPP
#define STRIPPEDCONFIGDIALOG_HPP STRIPPEDCONFIGDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */
#include <MagicEncoderLoader.hpp>
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QTabWidget;

/* forward declaration of local classes */
class CDReader;
class ConfigNotifyWidget;
class Encoder;
class GlobalConfigWidget;
class ProxyWidget;

/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief

 \dotfile "graphs/apps/Stripped/StrippedConfigDialog_connect.dot" "Connect Graph"
*/
class StrippedConfigDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param cdreader
    \param parent
    \param flags
   */
   StrippedConfigDialog( CDReader *cdreader, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~StrippedConfigDialog();

   /*!
    \brief \todo

    \return QList<QAction *>
   */
   QList<QAction*> encoderToggleEnableActions();

public slots:

   /*!
    \brief execute the dialog

   */
   void exec();

   /*!
    \brief set the directory where the ripped tracks go to

   */
   void setRippingDir();

   /*!
    \brief read settings from storage

   */
   void readSettings();

   /*!
    \brief write settings to storage

   */
   void writeSettings();

   /*!
    \brief update example

    \param text
   */
   void updatePattern( const QString &text );

   /*!
    \brief handle the list of all devices available for ripping

    \param devices
   */
   void handleDevices( const QStringList &devices );

   /*!
    \brief handle the selection of the actual device used for ripping

    \param device
   */
   void handleDevice( const QString &device );

signals:
   /*!
    \brief notify that no drive is available

   */
   void stateNoDrive();

   /*!
    \brief notify that the configuration has changed

   */
   void configChanged();

private:
   Q_DISABLE_COPY( StrippedConfigDialog )

   CDReader             *mpCDReader; /*!< \todo */
   GlobalConfigWidget   *mpGlobalConfigWidget; /*!< \todo */
   ProxyWidget          *mpProxyWidget; /*!< \todo */
   QLabel               *mpDevicesLabel; /*!< \todo */
   QComboBox            *mpDevicesBox; /*!< \todo */
   QCheckBox            *mpAutoFreeDB; /*!< \todo */
   QCheckBox            *mpAutoEject; /*!< \todo */
   QCheckBox            *mpAutoEnqueue; /*!< \todo */
   QCheckBox            *mpCDTextLatin1; /*!< \todo */
   QLabel               *mpDirButtonLabel; /*!< \todo */
   QLineEdit            *mpDirEdit; /*!< \todo */
   QPushButton          *mpDirButton; /*!< \todo */
   QLabel               *mpPatternLabel; /*!< \todo */
   QLineEdit            *mpPattern; /*!< \todo */
   QLabel               *mpPatternExample; /*!< \todo */
   QTabWidget           *mpEncoderTabs; /*!< \todo */
   TagList              mTagList; /*!< \todo */
   MagicEncoderList     mEncoders; /*!< \todo */
};

/* defaults */

/*! @} */

#endif
