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

/* local library headers */
#include <MagicEncoderLoader.hpp>
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
class ProxyWidget;


class ConfigDialog : public QDialog
{
Q_OBJECT

public:
   ConfigDialog( CDReader *cdreader, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~ConfigDialog();

public slots:
   /* execute the dialog */
   void exec();
   /* set the directory where the ripped tracks go to */
   void setRippingDir();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* update example */
   void updatePattern( const QString &text );
   /* handle the list of all devices available for ripping */
   void handleDevices( const QStringList &devices );
   /* handle the selection of the actual device used for ripping */
   void handleDevice( const QString &device );

signals:
   /* notify that no drive is available */
   void stateNoDrive();
   /* notify that the configuration has changed */
   void configChanged();

private:
   ConfigDialog( const ConfigDialog &that );
   ConfigDialog &operator=( const ConfigDialog &that );

   CDReader             *mpCDReader;
   GlobalConfigWidget   *mpGlobalConfigWidget;
   ProxyWidget          *mpProxyWidget;
   QLabel               *mpDevicesLabel;
   QComboBox            *mpDevicesBox;
   QCheckBox            *mpAutoFreeDB;
   QCheckBox            *mpAutoEject;
   QCheckBox            *mpAutoEnqueue;
   QCheckBox            *mpShowStats;
   QLabel               *mpDirButtonLabel;
   QLineEdit            *mpDirEdit;
   QPushButton          *mpDirButton;
   QLabel               *mpPatternLabel;
   QLineEdit            *mpPattern;
   QLabel               *mpPatternExample;
   QTabWidget           *mpEncoderTabs;
   TagList              mTagList;
   MagicEncoderList     mEncoders;
};

/* defaults */
#define VALUE_AUTOEJECT          value( "AutoEject", false ).toBool()
#define VALUE_AUTOENQUEUE        value( "AutoEnQueue", false ).toBool()
#define VALUE_AUTOFREEDB         value( "AutoFreeDB", true ).toBool()
#define VALUE_CREATEPATTERN      value( "CreatePattern", "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString()
#define VALUE_DEVICE             value( "Device", QString("/dev/cdrom") ).toString()
#define VALUE_DIRECTORY          value( "Directory", QDir::current().absolutePath() ).toString()
#define VALUE_DIRECTORY_OVERRIDE value( "DirectoryOverride", false ).toBool()
#define VALUE_FLACQUALITY        value( "FlacQuality", 5 ).toInt()
#define VALUE_FLACUSEOGA         value( "FlacUseOga", false ).toBool()
#define VALUE_OGGQUALITY         value( "OggQuality", 0.4 ).toDouble()
#define VALUE_SHOWSTATS          value( "ShowStats", false ).toBool()
#define VALUE_STYLESHEET         value( "StyleSheet", QString() ).toString()
#define VALUE_USE_ENCODER        value( "UseEncoder", false ).toBool()
#define VALUE_USE_LATIN1         value( "UseLatin1", false ).toBool()
#define VALUE_VBRQUALITY         value( "VBRQuality", 4.0 ).toDouble()

#endif
