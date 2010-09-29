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

/* local headers */
#include "CDReader.hpp"

#include <Trace.hpp>


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
, mpShowStats( new QCheckBox( tr("Show CD reading status"), this ) )
, mpDirButtonLabel( new QLabel( tr("Base Directory:"), this ) )
, mpDirButton( new QPushButton( this ) )
, mpPatternLabel( new QLabel( tr("File Create Pattern:"), this ) )
, mpPattern( new QLineEdit( this ) )
, mpPatternExample( new QLabel( tr(""), this ) )
, mpEncoderTabs( new QTabWidget( this ) )
, mTagList()
, mEncoders()
{
   int i = 0;
   int encodersActive = 0;
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   setWindowIcon( QIcon(":/SLART.png") );

   mEncoders = MagicEncoderLoader::tryLoading( "s0d" );
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
qDebug() << "loading:" << mEncoders.size();
   for( i = 0; i < mEncoders.size(); i++ )
   {
qDebug() << "loading:" << mEncoders.at(i)->name();
      QCheckBox *encoderCheckBox = new QCheckBox( mEncoders.at(i)->name(), encoders );
      mpEncoderTabs->addTab( mEncoders.at(i)->configWidget( this, encoderCheckBox ), mEncoders.at(i)->name() );
      if( mEncoders.at(i)->useEncoder() )
      {
         encoderCheckBox->setChecked( true );
         encodersActive++;
      }
      encodersLayout->addWidget( encoderCheckBox, i + 1, 0 );
      encodersLayout->addWidget( new QLabel(
         QString("(%1)").arg(mEncoders.at(i)->pluginFileName()), this ), i + 1, 1 );
   }
   encodersLayout->setRowStretch( i + 1, 1 );
   encodersLayout->setColumnStretch( 1, 1 );
   readSettings();

   if( !encodersActive )
   {
      QTimer::singleShot( 100, this, SLOT(exec()) );
   }

   QWidget     *strTab    = new QWidget( this );
   QGridLayout *strLayout = new QGridLayout( strTab );
   strLayout->addWidget( mpDevicesLabel,   0, 0 );
   strLayout->addWidget( mpDevicesBox,     0, 1 );
   strLayout->addWidget( mpDirButtonLabel, 1, 0 );
   strLayout->addWidget( mpDirButton,      1, 1 );
   strLayout->addWidget( mpPatternLabel,   2, 0 );
   strLayout->addWidget( mpPattern,        2, 1 );
   strLayout->addWidget( mpPatternExample, 3, 1 );
   strLayout->addWidget( mpAutoFreeDB,     4, 0, 1, 2 );
   strLayout->addWidget( mpAutoEject,      5, 0, 1, 2 );
   strLayout->addWidget( mpAutoEnqueue,    6, 0, 1, 2 );
   strLayout->addWidget( mpShowStats,      7, 0, 1, 2 );
   strLayout->addWidget( mpEncoderTabs,    8, 0, 1, 2 );
   strLayout->setRowStretch( 8, 1 );
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

   connect( mpCDReader, SIGNAL(foundDevices(const QStringList &)),
            this, SLOT(handleDevices(const QStringList &)) );
   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setRippingDir()) );
   connect( mpPattern, SIGNAL(textChanged(const QString&)),
            this, SLOT(updatePattern(const QString &)) );
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   
   mpCDReader->getDevices();
}


ConfigDialog::~ConfigDialog()
{
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      if( mEncoders.at(i)->workerThread()->isRunning() )
      {
         mEncoders.at(i)->workerThread()->quit();
         mEncoders.at(i)->workerThread()->wait();
      }
      delete mEncoders.at(i);
   }
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();

   int encodersActive = 0;
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      if( mEncoders.at(i)->useEncoder() )
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
   fileDialog.setDirectory( mpDirButton->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      MySettings settings;
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirButton->setText( result );
   }
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   int i;
   i = mpDevicesBox->findText( settings.VALUE_DEVICE );
   if( i >= 0 )
   {
      mpDevicesBox->setCurrentIndex( i );
   }
   mpAutoFreeDB->setChecked( settings.VALUE_AUTOFREEDB );
   mpAutoEject->setChecked( settings.VALUE_AUTOEJECT );
   mpAutoEnqueue->setChecked( settings.VALUE_AUTOENQUEUE );
   mpShowStats->setChecked( settings.VALUE_SHOWSTATS );
   if( i < 0 )
   {
      i = 0;
   }
   mpDirButton->setText( settings.VALUE_DIRECTORY.replace( '/', QDir::separator() ) );
   mpPattern->setText( settings.VALUE_CREATEPATTERN );
   
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->configWidget()->readSettings();
   }
   mpCDReader->setEncoders( mEncoders );
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   settings.setValue( "Device", mpDevicesBox->currentText() );
   settings.setValue( "AutoFreeDB", mpAutoFreeDB->isChecked() );
   settings.setValue( "AutoEject", mpAutoEject->isChecked() );
   settings.setValue( "AutoEnqueue", mpAutoEnqueue->isChecked() );
   settings.setValue( "ShowStats", mpShowStats->isChecked() );
   settings.setValue( "CreatePattern", mpPattern->text() );
   settings.setValue( "Directory", mpDirButton->text().replace('\\','/') );
   
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->configWidget()->writeSettings();
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
   disconnect( mpDevicesBox, SIGNAL(currentIndexChanged(const QString&)),
               this, SLOT(handleDevice(const QString&)) );
   mpDevicesBox->clear();
   if( devices.size() )
   {
      mpDevicesBox->addItems( devices );
      mpDevicesBox->setCurrentIndex( mpDevicesBox->findText( MySettings().VALUE_DEVICE ) );
      mpDevicesBox->insertSeparator( mpDevicesBox->count() );
   }
   else
   {
      emit stateNoDrive();
   }
   mpDevicesBox->addItem( tr("Rescan") );
   connect( mpDevicesBox, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(handleDevice(const QString&)) );
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
