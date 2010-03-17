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
, mpPatternLabel( new QLabel( tr("File Create Pattern"), this ) )
, mpPattern( new QLineEdit( this ) )
, mpPatternExample( new QLabel( tr(""), this ) )
, mpEncodersLabel( new QLabel( tr("Encoder:"), this ) )
, mpEncodersBox( new QComboBox( this ) )
, mTagList()
, mEncoders()
{
   int i;
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   setWindowIcon( QIcon(":/SLART.png") );

   mEncoders.append( new OggEncoder( this ) );
   mEncoders.append( new WavEncoder( this ) );
   mEncoders.append( new FlacEncoder( this ) );
   
   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","Album_Artist");
   mTagList.set("ALBUM","Album");
   mTagList.set("ARTIST","Artist");
   mTagList.set("TITLE","Title");
   mTagList.set("GENRE","Genre");
   mTagList.set("Year","1986");

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showNormalize();
   
   QWidget     *strTab    = new QWidget( this );
   QGridLayout *strLayout = new QGridLayout( strTab );
   strLayout->addWidget( mpDevicesLabel,   0, 0 );
   strLayout->addWidget( mpDevicesBox,     0, 1 );
   strLayout->addWidget( mpPatternLabel,   1, 0 );
   strLayout->addWidget( mpPattern,        1, 1 );
   strLayout->addWidget( mpPatternExample, 2, 1 );
   strLayout->addWidget( mpAutoFreeDB,     3, 0, 1, 2 );
   strLayout->addWidget( mpEncodersLabel,  4, 0 );
   strLayout->addWidget( mpEncodersBox,    4, 1 );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      strLayout->addWidget( new QLabel( (mEncoders.at(i)->mName + tr(" Encoder") ) ), 5+i, 0 );
      strLayout->addWidget( mEncoders.at(i)->configWidget(), 5+i, 1 );
      mpEncodersBox->addItem( mEncoders.at(i)->mName );
   }
   strLayout->setRowStretch( 4+i, 1 );
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
   connect( mpEncodersBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeEncoder(int)) );
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
   
   readSettings();
   mpCDReader->getDevices();
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
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
   i = mpEncodersBox->findText( settings.VALUE_ENCODER );
   if( i < 0 )
   {
      i = 0;
   }
   mpEncodersBox->setCurrentIndex( i );
   changeEncoder( i );
   mpPattern->setText( settings.VALUE_CREATEPATTERN );
   
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->readSettings();
   }
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   settings.setValue( "Device", mpDevicesBox->currentText() );
   settings.setValue( "AutoFreeDB", mpAutoFreeDB->isChecked() );
   settings.setValue( "CreatePattern", mpPattern->text() );
   settings.setValue( "Encoder", mpEncodersBox->currentText() );
   
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->writeSettings();
   }

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


void ConfigDialog::changeEncoder( int id )
{
   mpCDReader->setEncoder( mEncoders.at(id) );
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->configWidget()->setDisabled( i != id );
   }
}


void ConfigDialog::updatePattern( const QString &text )
{
   mpPatternExample->setText( mTagList.fileName(text)+".ext" );
}
