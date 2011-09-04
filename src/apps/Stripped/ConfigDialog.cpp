/**
 * src/apps/Stripped/ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <MagicEncoderInterface.hpp>
#include <MagicEncoderProxy.hpp>
#include <MagicEncoder/MagicEncoderConfig.hpp>
#include <MySettings.hpp>
#include <ProxyWidget.hpp>
#include <Satellite.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "CDReader.hpp"

/* class variable instantiation */
QPointer<ConfigDialog> ConfigDialog::cpConfigDialog = 0;


ConfigDialog *ConfigDialog::createGlobal( CDReader *cdreader, QWidget *parent, Qt::WindowFlags flags )
{
   if( !cpConfigDialog )
   {
      cpConfigDialog = new ConfigDialog( cdreader, parent, flags );
   }
   else
   {
      cpConfigDialog->mpCDReader = cdreader;
      cdreader->getDevices();
      connect( cdreader, SIGNAL(foundDevices(QStringList)),
               cpConfigDialog, SLOT(handleDevices(QStringList)) );
      foreach( MagicEncoderProxy *encoder, cpConfigDialog->mEncoders )
      {
         encoder->configWidget()->readSettings();
      }
      cdreader->setEncoders( cpConfigDialog->mEncoders );
   }
   return cpConfigDialog;
}


ConfigDialog *ConfigDialog::get()
{
   return cpConfigDialog;
}


void ConfigDialog::destroyGlobal()
{
   if( cpConfigDialog )
   {
      delete cpConfigDialog;
      cpConfigDialog = 0;
   }
}


ConfigDialog::ConfigDialog( CDReader *cdreader, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpCDReader( cdreader )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mpDevicesLabel( new QLabel( tr("Device:"), this ) )
, mpDevicesBox( new QComboBox( this ) )
, mpAutoFreeDB( new QCheckBox( tr("Automatically run FreeDB query"), this ) )
, mpAutoEject( new QCheckBox( tr("Automatically eject CD when done"), this ) )
, mpAutoEnqueue( new QCheckBox( tr("Automatically enqueue tracks with errors in Partyman"), this ) )
, mpCDTextLatin1( new QCheckBox( tr("Assume CD-Text is encoded as latin-1 instead of utf-8"), this ) )
, mpShowStats( new QCheckBox( tr("Show CD reading status"), this ) )
, mpDirButtonLabel( new QLabel( tr("Base Directory:"), this ) )
, mpDirEdit( new QLineEdit( this ) )
, mpDirButton( new QPushButton( "...", this ) )
, mpPatternLabel( new QLabel( tr("File Create Pattern:"), this ) )
, mpPattern( new QLineEdit( this ) )
, mpPatternExample( new QLabel( tr(""), this ) )
, mpEncoderTabs( new QTabWidget( this ) )
, mpSettings( new MySettings( this ) )
, mTagList()
, mEncoders()
{
   cpConfigDialog = this;
   int i = 0;
   int encodersActive = 0;
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon(":/SLART.png") );

   QCompleter *completer = new QCompleter( this );
   completer->setModel( new QDirModel( QStringList(),
                                       QDir::NoDotAndDotDot | QDir::AllDirs,
                                       QDir::Name,
                                       completer ) );
   mpDirEdit->setCompleter( completer );
   /* evil hack */
   mpDirButton->setMaximumWidth( mpDirButton->height() );

   mEncoders = MagicEncoderLoader::tryLoading( "s0d", Satellite::get() );
   if( mEncoders.size() == 0 )
   {
      QMessageBox::critical( this,
                             tr("%1: Fatal Error").arg(QApplication::applicationName()),
                             tr("Could not find any encoder plugins.") );
      exit( 1 );
   }

   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","Album_Artist");
   mTagList.set("ALBUM","Album");
   mTagList.set("ARTIST","Artist");
   mTagList.set("TITLE","Title");
   mTagList.set("GENRE","Genre");
   mTagList.set("Year","1986");

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showNormalize();

   QWidget *encoders = new QWidget( this );
   QGridLayout *encodersLayout = new QGridLayout( encoders );
   encodersLayout->addWidget( new QLabel(
      tr("Encoders used during ripping (select at least one):"), encoders ), 0, 0, 1, 2 );
   mpEncoderTabs->addTab( encoders, tr("Encoders") );
   i=1;
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      QCheckBox *encoderCheckBox = new QCheckBox( encoder->name(), encoders );
      mpEncoderTabs->addTab( encoder->configWidget( this, encoderCheckBox ), encoder->name() );
      if( encoder->useEncoder() )
      {
         encoderCheckBox->setChecked( true );
         encodersActive++;
      }
      encodersLayout->addWidget( encoderCheckBox, i, 0 );
      encodersLayout->addWidget( new QLabel(
         QString("(%1)").arg(encoder->pluginFileName()), this ), i, 1 );
      i++;
   }
   encodersLayout->setRowStretch( i, 1 );
   encodersLayout->setColumnStretch( 1, 1 );
   readSettings();

   if( !encodersActive )
   {
      QTimer::singleShot( 100, this, SLOT(exec()) );
   }

   QWidget     *strTab    = new QWidget( this );
   QGridLayout *strLayout = new QGridLayout( strTab );
   strLayout->addWidget( mpDevicesLabel,   0, 0 );
   strLayout->addWidget( mpDevicesBox,     0, 1, 1, 2 );
   strLayout->addWidget( mpDirButtonLabel, 1, 0 );
   strLayout->addWidget( mpDirEdit,        1, 1 );
   strLayout->addWidget( mpDirButton,      1, 2 );
   strLayout->addWidget( mpPatternLabel,   2, 0 );
   strLayout->addWidget( mpPattern,        2, 1, 1, 2 );
   strLayout->addWidget( mpPatternExample, 3, 1, 1, 2 );
   strLayout->addWidget( mpAutoFreeDB,     4, 0, 1, 3 );
   strLayout->addWidget( mpAutoEject,      5, 0, 1, 3 );
   strLayout->addWidget( mpAutoEnqueue,    6, 0, 1, 3 );
   strLayout->addWidget( mpCDTextLatin1,   7, 0, 1, 3 );
   strLayout->addWidget( mpShowStats,      8, 0, 1, 3 );
   strLayout->addWidget( mpEncoderTabs,    9, 0, 1, 3 );
   strLayout->setColumnStretch( 2, 1 );
   strLayout->setRowStretch( 9, 1 );
   strTab->setLayout( strLayout );

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( strTab,               tr("Stripped") );
   tabs->addTab( mpProxyWidget,        tr("Proxy") );
   tabs->addTab( mpGlobalConfigWidget, tr("Global") );

   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );

   setLayout( mainLayout );

   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setRippingDir()) );
   connect( mpPattern, SIGNAL(textChanged(QString)),
            this, SLOT(updatePattern(QString)) );
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );

   if( mpCDReader )
   {
      mpCDReader->getDevices();
      connect( mpCDReader, SIGNAL(foundDevices(QStringList)),
               this, SLOT(handleDevices(QStringList)) );
   }
   WidgetShot::addWidget( "ConfigDialog", this );
}


ConfigDialog::~ConfigDialog()
{
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      if( encoder->workerThread()->isRunning() )
      {
         encoder->workerThread()->quit();
         encoder->workerThread()->wait();
      }
      delete encoder;
   }
}


bool ConfigDialog::value( enum ParameterBool id )
{
   switch( id )
   {
   case ParameterAutoEject:
      return cpConfigDialog->mpSettings->value( "AutoEject", false ).toBool();
   case ParameterAutoEnqueue:
      return cpConfigDialog->mpSettings->value( "AutoEnqueue", false ).toBool();
   case ParameterAutoFreeDB:
      return cpConfigDialog->mpSettings->value( "AutoFreeDB", true ).toBool();
   case ParameterCDTextLatin1:
      return cpConfigDialog->mpSettings->value( "CDTextLatin1", false ).toBool();
   case ParameterDirectoryOverride:
      return cpConfigDialog->mpSettings->value( "DirectoryOverride", false ).toBool();
   case ParameterShowStats:
      return cpConfigDialog->mpSettings->value( "ShowStats", false ).toBool();
   default:
      qFatal( "illegal ParameterBool value" );
      return false;
   }
}


void ConfigDialog::setValue( enum ParameterBool id, bool value )
{
   switch( id )
   {
   case ParameterAutoEject:
      cpConfigDialog->mpSettings->setValue( "AutoEject", value );
   case ParameterAutoEnqueue:
      cpConfigDialog->mpSettings->setValue( "AutoEnqueue", value );
   case ParameterAutoFreeDB:
      cpConfigDialog->mpSettings->setValue( "AutoFreeDB", value );
   case ParameterCDTextLatin1:
      cpConfigDialog->mpSettings->setValue( "CDTextLatin1", value );
   case ParameterDirectoryOverride:
      cpConfigDialog->mpSettings->setValue( "DirectoryOverride", value );
   case ParameterShowStats:
      cpConfigDialog->mpSettings->setValue( "ShowStats", value );
   default:
      qFatal( "illegal ParameterBool value" );
   }
}


QString ConfigDialog::value( enum ParameterString id )
{
   switch( id )
   {
   case ParameterCreatePattern:
      return cpConfigDialog->mpSettings->value( "CreatePattern", "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString();
   case ParameterDevice:
      return cpConfigDialog->mpSettings->value( "Device", QString("/dev/cdrom") ).toString();
   case ParameterDirectory:
      return cpConfigDialog->mpSettings->value( "Directory", QDir::current().absolutePath() ).toString();
   case ParameterStyleSheet:
      return cpConfigDialog->mpSettings->value( "StyleSheet", QString() ).toString();
   default:
      qFatal( "illegal ParameterString value" );
      return QString();
   }
}


void ConfigDialog::setValue( enum ParameterString id, const QString &value )
{
   switch( id )
   {
   case ParameterCreatePattern:
      cpConfigDialog->mpSettings->setValue( "CreatePattern", value );
   case ParameterDevice:
      cpConfigDialog->mpSettings->setValue( "Device", value );
   case ParameterDirectory:
      cpConfigDialog->mpSettings->setValue( "Directory", value );
   case ParameterStyleSheet:
      cpConfigDialog->mpSettings->setValue( "StyleSheet", value );
   default:
      qFatal( "illegal ParameterString value" );
   }
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();

   int encodersActive = 0;
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      if( encoder->useEncoder() )
      {
         encodersActive++;
      }
   }
   if( !encodersActive )
   {
      mpEncoderTabs->setCurrentIndex( 0 );
      QTimer::singleShot( 100, this, SLOT(exec()) );
   }
}


void ConfigDialog::setRippingDir()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpDirEdit->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      MySettings settings;
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirEdit->setText( result );
   }
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   int i;
   i = mpDevicesBox->findText( value( ParameterDevice ) );
   if( i >= 0 )
   {
      mpDevicesBox->setCurrentIndex( i );
   }
   mpAutoFreeDB->setChecked( value( ParameterAutoFreeDB ) );
   mpAutoEject->setChecked( value( ParameterAutoFreeDB ) );
   mpAutoEnqueue->setChecked( value( ParameterAutoEnqueue ) );
   mpCDTextLatin1->setChecked( value( ParameterCDTextLatin1 ) );
   mpShowStats->setChecked( value( ParameterShowStats ) );
   if( i < 0 )
   {
      i = 0;
   }
   mpDirEdit->setText( value( ParameterDirectory ).replace( '/', QDir::separator() ) );
   mpPattern->setText( value( ParameterCreatePattern ) );

   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      encoder->configWidget()->readSettings();
   }
   if( mpCDReader )
   {
      mpCDReader->setEncoders( mEncoders );
   }

   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   cpConfigDialog->mpSettings->setValue( "Device", mpDevicesBox->currentText() );
   cpConfigDialog->mpSettings->setValue( "AutoFreeDB", mpAutoFreeDB->isChecked() );
   cpConfigDialog->mpSettings->setValue( "AutoEject", mpAutoEject->isChecked() );
   cpConfigDialog->mpSettings->setValue( "AutoEnqueue", mpAutoEnqueue->isChecked() );
   cpConfigDialog->mpSettings->setValue( "CDTextLatin1", mpCDTextLatin1->isChecked() );
   cpConfigDialog->mpSettings->setValue( "ShowStats", mpShowStats->isChecked() );
   cpConfigDialog->mpSettings->setValue( "CreatePattern", mpPattern->text() );
   cpConfigDialog->mpSettings->setValue( "Directory", mpDirEdit->text().replace('\\','/') );

   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      encoder->configWidget()->writeSettings();
   }
   mpCDReader->setEncoders( mEncoders );

   emit configChanged();
}


void ConfigDialog::handleDevices( const QStringList &devices )
{
#if 0
TRACESTART(ConfigDialog::handleDevices);
TRACEMSG << devices;
#endif
   disconnect( mpDevicesBox, SIGNAL(currentIndexChanged(QString)),
               this, SLOT(handleDevice(QString)) );
   mpDevicesBox->clear();
   if( devices.size() )
   {
      mpDevicesBox->addItems( devices );
      mpDevicesBox->setCurrentIndex( mpDevicesBox->findText( value( ParameterDevice ) ) );
      mpDevicesBox->insertSeparator( mpDevicesBox->count() );
   }
   else
   {
      emit stateNoDrive();
   }
   mpDevicesBox->addItem( tr("Rescan") );
   connect( mpDevicesBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleDevice(QString)) );
}


void ConfigDialog::handleDevice( const QString &device )
{
#if 0
TRACESTART(ConfigDialog::handleDevice);
TRACEMSG << device;
#endif
   if( mpDevicesBox->findText( device ) == mpDevicesBox->count() - 1 )
   {
      mpCDReader->getDevices();
   }
   else
   {
      mpCDReader->setDevice( device );
   }
}


void ConfigDialog::updatePattern( const QString &text )
{
   mpPatternExample->setText( mTagList.fileName(text)+".ext" );
}
