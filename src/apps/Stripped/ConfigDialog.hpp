/**
 * src/apps/Stripped/ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */
#include <QPointer>

/* local library headers */
#include <MagicEncoderLoader.hpp>
#include <MySettings.hpp>
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
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
class MySettings;
class ProxyWidget;

/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief

*/
class ConfigDialog : public QDialog
{
   Q_OBJECT
   Q_ENUMS(ParameterBool ParameterString)

public:
   /*!
    \brief parameter for value() / setValue (for boolean types)

   */
   enum ParameterBool
   {
      ParameterAutoEject, ParameterAutoEnqueue, ParameterAutoFreeDB,
      ParameterCDTextLatin1, ParameterDirectoryOverride, ParameterShowStats
   };
   /*!
    \brief parameter for value() / setValue (for string types)

   */
   enum ParameterString
   {
      ParameterCreatePattern, ParameterDevice, ParameterDirectory,
      ParameterStyleSheet
   };

   /*!
    \brief create and get global instance

    \param cdreader
    \param parent
    \param flags
   */
   static ConfigDialog *createGlobal( CDReader *cdreader = 0, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief get global instance

   */
   static ConfigDialog *get();

   /*!
    \brief destroy global instance

   */
   static void destroyGlobal();

   /*!
    \brief get value from settings storage

    \param id key to get value for
   */
   static bool value( enum ParameterBool id );
   /*!
    \brief set value from settings storage

    \param id key to set value for
    \param value value to set
   */
   static void setValue( enum ParameterBool id, bool value );
   /*!
    \brief get value from settings storage

    \param id key to get value for
   */
   static QString value( enum ParameterString id );
   /*!
    \brief set value from settings storage

    \param id key to set value for
    \param value value to set
   */
   static void setValue( enum ParameterString id, const QString &value );

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
   /*!
    \brief constructor

    \param cdreader
    \param parent
    \param flags
   */
   ConfigDialog( CDReader *cdreader, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~ConfigDialog();

   Q_DISABLE_COPY( ConfigDialog )

   CDReader             *mpCDReader; /*!< TODO */
   GlobalConfigWidget   *mpGlobalConfigWidget; /*!< TODO */
   ProxyWidget          *mpProxyWidget; /*!< TODO */
   QLabel               *mpDevicesLabel; /*!< TODO */
   QComboBox            *mpDevicesBox; /*!< TODO */
   QCheckBox            *mpAutoFreeDB; /*!< TODO */
   QCheckBox            *mpAutoEject; /*!< TODO */
   QCheckBox            *mpAutoEnqueue; /*!< TODO */
   QCheckBox            *mpCDTextLatin1; /*!< TODO */
   QCheckBox            *mpShowStats; /*!< TODO */
   QLabel               *mpDirButtonLabel; /*!< TODO */
   QLineEdit            *mpDirEdit; /*!< TODO */
   QPushButton          *mpDirButton; /*!< TODO */
   QLabel               *mpPatternLabel; /*!< TODO */
   QLineEdit            *mpPattern; /*!< TODO */
   QLabel               *mpPatternExample; /*!< TODO */
   QTabWidget           *mpEncoderTabs; /*!< TODO */
   MySettings           *mpSettings;
   TagList              mTagList; /*!< TODO */
   MagicEncoderList     mEncoders; /*!< TODO */

   static QPointer<ConfigDialog> cpConfigDialog; /*!< TODO */
};

/* defaults */
#if 0
#define VALUE_AUTOEJECT          value( "AutoEject", false ).toBool()
#define VALUE_AUTOENQUEUE        value( "AutoEnQueue", false ).toBool()
#define VALUE_AUTOFREEDB         value( "AutoFreeDB", true ).toBool()
#define VALUE_CDTEXT_LATIN1      value( "CDTextLatin1", false ).toBool()
#define VALUE_CREATEPATTERN      value( "CreatePattern", "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString()
#define VALUE_DEVICE             value( "Device", QString("/dev/cdrom") ).toString()
#define VALUE_DIRECTORY          value( "Directory", QDir::current().absolutePath() ).toString()
#define VALUE_DIRECTORY_OVERRIDE value( "DirectoryOverride", false ).toBool()
#define VALUE_SHOWSTATS          value( "ShowStats", false ).toBool()
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()
#endif

/*! @} */

#endif
