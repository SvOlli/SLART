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
#include <MySettings.hpp>
#include <ProxyWidget.hpp>

/* local headers */
#include "CDReader.hpp"
#include "FlacEncoder.hpp"
#if USE_MP3
#include "Mp3Encoder.hpp"
#endif
#include "OggEncoder.hpp"
#include "WavEncoder.hpp"

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
, mpPatternLabel( new QLabel( tr("File Create Pattern"), this ) )
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

#if USE_MP3
   mEncoders.append( new Mp3Encoder( this ) );
#endif
   mEncoders.append( new OggEncoder( this ) );
   mEncoders.append( new FlacEncoder( this ) );
   mEncoders.append( new WavEncoder( this ) );
   
   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","Album_Artist");
   mTagList.set("ALBUM","Album");
   mTagList.set("ARTIST","Artist");
   mTagList.set("TITLE","Title");
   mTagList.set("GENRE","Genre");
   mTagList.set("Year","1986");

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showNormalize();
   
   readSettings();
   QWidget *encoders = new QWidget( this );
   QBoxLayout *encodersLayout = new QVBoxLayout( encoders );
   encodersLayout->addWidget( new QLabel( tr("Encoders used during ripping (select at least one):"), encoders ) );
   mpEncoderTabs->addTab( encoders, tr("Encoders") );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mpEncoderTabs->addTab( mEncoders.at(i)->configWidget(), mEncoders.at(i)->mName );
      QCheckBox *encoder = new QCheckBox( mEncoders.at(i)->mName, encoders );
      if( mEncoders.at(i)->useEncoder() )
      {
         encoder->setChecked( true );
         encodersActive++;
      }
      encodersLayout->addWidget( encoder );
      connect( encoder, SIGNAL(clicked(bool)),
               mEncoders.at(i), SLOT(setUseEncoder(bool)) );
      connect( mEncoders.at(i), SIGNAL(useEncoderClicked(bool)),
               encoder, SLOT(setChecked(bool)) );
   }
   encodersLayout->addStretch( 1 );
   if( !encodersActive )
   {
      QTimer::singleShot( 100, this, SLOT(exec()) );
   }

   QWidget     *strTab    = new QWidget( this );
   QGridLayout *strLayout = new QGridLayout( strTab );
   strLayout->addWidget( mpDevicesLabel,   0, 0 );
   strLayout->addWidget( mpDevicesBox,     0, 1 );
   strLayout->addWidget( mpPatternLabel,   1, 0 );
   strLayout->addWidget( mpPattern,        1, 1 );
   strLayout->addWidget( mpPatternExample, 2, 1 );
   strLayout->addWidget( mpAutoFreeDB,     3, 0, 1, 2 );
   strLayout->addWidget( mpAutoEject,      4, 0, 1, 2 );
   strLayout->addWidget( mpEncoderTabs,    5, 0, 1, 2 );
   strLayout->setRowStretch( 5, 1 );
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

   connect( mpDevicesBox, SIGNAL(currentIndexChanged(const QString&)),
            mpCDReader, SLOT(setDevice(const QString&)) );
   connect( mpCDReader, SIGNAL(foundDevices(const QStringList &)),
            this, SLOT(handleDevices(const QStringList &)) );
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   connect( mpPattern, SIGNAL(textChanged(const QString&)),
            this, SLOT(updatePattern(const QString &)) );
   
   mpCDReader->getDevices();
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
      QTimer::singleShot( 100, this, SLOT(exec()) );
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
   if( i < 0 )
   {
      i = 0;
   }
   mpPattern->setText( settings.VALUE_CREATEPATTERN );
   
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->readSettings();
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
   settings.setValue( "CreatePattern", mpPattern->text() );
   
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->writeSettings();
   }
   mpCDReader->setEncoders( mEncoders );

   emit configChanged();
}


void ConfigDialog::handleDevices( const QStringList &devices )
{
   mpDevicesBox->clear();
   if( devices.size() )
   {
      mpDevicesBox->addItems( devices );
      mpDevicesBox->setCurrentIndex( mpDevicesBox->findText( MySettings().VALUE_DEVICE ) );
   }
   else
   {
      emit stateNoDrive();
   }
}


void ConfigDialog::updatePattern( const QString &text )
{
   mpPatternExample->setText( mTagList.fileName(text)+".ext" );
}
