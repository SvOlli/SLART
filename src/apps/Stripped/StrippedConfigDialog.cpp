/*
 * src/apps/Stripped/StrippedConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "StrippedConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTimer>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <MagicEncoderInterface.hpp>
#include <MagicEncoderProxy.hpp>
#include <MagicEncoder/MagicEncoderConfig.hpp>
#include <ProxyWidget.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "CDReader.hpp"

/* class variable instantiation */


StrippedConfigDialog::StrippedConfigDialog( CDReader *cdreader, QWidget *parent, Qt::WindowFlags flags )
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
, mpDirButtonLabel( new QLabel( tr("Base Directory:"), this ) )
, mpDirEdit( new QLineEdit( this ) )
, mpDirButton( new QPushButton( "...", this ) )
, mpPatternLabel( new QLabel( tr("File Create Pattern:"), this ) )
, mpPattern( new QLineEdit( this ) )
, mpPatternExample( new QLabel( tr(""), this ) )
, mpEncoderTabs( new QTabWidget( this ) )
, mpCDDBUrlLabel( new QLabel( tr("CDDB URL:"), this ) )
, mpCDDBUrl( new QLineEdit( this ) )
, mpCDDBEmailLabel( new QLabel( tr("CDDB EMail:"), this ) )
, mpCDDBEmail( new QLineEdit( this ) )
, mTagMap()
, mEncoders()
{
   int i = 0;
   int encodersActive = 0;
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon(":/Stripped/Icon.png") );

   QCompleter *completer = new QCompleter( this );
   QFileSystemModel *dirsModel = new QFileSystemModel( completer );
   dirsModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs );
   completer->setModel( dirsModel );
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

   mTagMap.insert("TRACKNUMBER","1");
   mTagMap.insert("ALBUMARTIST","Album_Artist");
   mTagMap.insert("ALBUM","Album");
   mTagMap.insert("ARTIST","Artist");
   mTagMap.insert("TITLE","Title");
   mTagMap.insert("GENRE","Genre");
   mTagMap.insert("Year","1986");

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
      mpEncoderTabs->addTab( encoder->configWidget( this ), encoder->name() );
      QCheckBox *encoderCheckBox = new QCheckBox( encoder->name(), encoders );
      QAction *toggleEnableAction = encoder->toggleEnableAction();
      connect( encoderCheckBox, SIGNAL(toggled(bool)),
               toggleEnableAction, SLOT(setChecked(bool)) );
      connect( toggleEnableAction, SIGNAL(toggled(bool)),
               encoderCheckBox, SLOT(setChecked(bool)) );
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
   strLayout->addWidget( mpEncoderTabs,    8, 0, 1, 3 );
   strLayout->setColumnStretch( 2, 1 );
   strLayout->setRowStretch( 8, 1 );
   strTab->setLayout( strLayout );

   QWidget     *cddbTab    = new QWidget( this );
   QGridLayout *cddbLayout = new QGridLayout( cddbTab );
   cddbLayout->addWidget( mpCDDBUrlLabel,  0, 0 );
   cddbLayout->addWidget( mpCDDBUrl,       0, 1 );
   cddbLayout->addWidget( mpCDDBEmailLabel,1, 0 );
   cddbLayout->addWidget( mpCDDBEmail,     1, 1 );
   cddbLayout->addWidget( new QLabel( this ), 2, 0, 1, 2 );
   strLayout->setColumnStretch( 1, 1 );
   strLayout->setRowStretch( 2, 1 );
   cddbTab->setLayout( cddbLayout );

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( strTab,               tr("Stripped") );
   tabs->addTab( cddbTab,              tr("CDDB") );
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
   WidgetShot::addWidget( "Config", this );
}


StrippedConfigDialog::~StrippedConfigDialog()
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


int StrippedConfigDialog::exec()
{
   readSettings();
   int retval = QDialog::exec();

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
//      QTimer::singleShot( 100, this, SLOT(exec()) );
   }

   return retval;
}


void StrippedConfigDialog::setRippingDir()
{
   QFileDialog fileDialog( this );

   fileDialog.setOption( QFileDialog::ShowDirsOnly, true );
   fileDialog.setDirectory( mpDirEdit->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirEdit->setText( result );
   }
}


void StrippedConfigDialog::readSettings()
{
   int i;
   i = mpDevicesBox->findText( Settings::value( Settings::StrippedDevice ) );
   if( i >= 0 )
   {
      mpDevicesBox->setCurrentIndex( i );
   }
   mpAutoFreeDB->setChecked( Settings::value( Settings::StrippedAutoFreeDB ) );
   mpAutoEject->setChecked( Settings::value( Settings::StrippedAutoEject ) );
   mpAutoEnqueue->setChecked( Settings::value( Settings::StrippedAutoEnqueue ) );
   mpCDTextLatin1->setChecked( Settings::value( Settings::StrippedCDTextLatin1 ) );
   if( i < 0 )
   {
      i = 0;
   }
   mpDirEdit->setText( Settings::value( Settings::StrippedDirectory ).replace( '/', QDir::separator() ) );
   mpPattern->setText( Settings::value( Settings::StrippedCreatePattern ) );
   mpCDDBUrl->setText( Settings::value( Settings::StrippedCDDBURL ) );
   mpCDDBEmail->setText( Settings::value( Settings::StrippedCDDBUser ) );

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


void StrippedConfigDialog::writeSettings()
{
   Settings::setValue( Settings::StrippedDevice, mpDevicesBox->currentText() );
   Settings::setValue( Settings::StrippedAutoFreeDB, mpAutoFreeDB->isChecked() );
   Settings::setValue( Settings::StrippedAutoEject, mpAutoEject->isChecked() );
   Settings::setValue( Settings::StrippedAutoEnqueue, mpAutoEnqueue->isChecked() );
   Settings::setValue( Settings::StrippedCDTextLatin1, mpCDTextLatin1->isChecked() );
   Settings::setValue( Settings::StrippedCreatePattern, mpPattern->text() );
   Settings::setValue( Settings::StrippedDirectory, mpDirEdit->text().replace('\\','/') );
   Settings::setValue( Settings::StrippedCDDBURL, mpCDDBUrl->text() );
   Settings::setValue( Settings::StrippedCDDBUser, mpCDDBEmail->text() );

   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      encoder->configWidget()->writeSettings();
   }
   mpCDReader->setEncoders( mEncoders );

   emit configChanged();
}


void StrippedConfigDialog::handleDevices( const QStringList &devices )
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
      mpDevicesBox->setCurrentIndex( mpDevicesBox->findText( Settings::value( Settings::StrippedDevice ) ) );
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


void StrippedConfigDialog::handleDevice( const QString &device )
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


void StrippedConfigDialog::updatePattern( const QString &text )
{
   mpPatternExample->setText( mTagMap.fileName(text)+".ext" );
}


QList<QAction*> StrippedConfigDialog::encoderToggleEnableActions()
{
   QList<QAction*> retval;
   foreach( MagicEncoderProxy *encoder, mEncoders )
   {
      retval << encoder->toggleEnableAction();
   }
   return retval;
}
