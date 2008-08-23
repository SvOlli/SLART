/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "GlobalConfigWidget.hpp"
#include "ProxyWidget.hpp"
#include "AboutWidget.hpp"


#include <QtGui>
#include "SLARTCom.hpp"
#include "CDReader.hpp"
#include "OggEncoder.hpp"
#include "RawEncoder.hpp"

#include "Trace.hpp"


ConfigDialog::ConfigDialog( CDReader *cdreader, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpCDReader( cdreader )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mpDevicesLabel( new QLabel( tr("Device:"), this ) )
, mpDevicesBox( new QComboBox( this ) )
, mpEncodersLabel( new QLabel( tr("Encoder:"), this ) )
, mpEncodersBox( new QComboBox( this ) )
, mpPatternLabel( new QLabel( tr("File Create Pattern"), this ) )
, mpPattern( new QLineEdit( this ) )
, mpPatternExample( new QLabel( tr(""), this ) )
, mTagList()
, mEncoders()
{
   int i;
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );

   mEncoders.append( new OggEncoder( this ) );
   mEncoders.append( new RawEncoder( this ) );
   
   mTagList.set("TRACKNUMBER","1");
   mTagList.set("ALBUMARTIST","AlbumArtist");
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
   strLayout->addWidget( mpEncodersLabel,  3, 0 );
   strLayout->addWidget( mpEncodersBox,    3, 1 );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      strLayout->addWidget( new QLabel( (mEncoders.at(i)->name + tr(" Encoder") ) ), 4+i, 0 );
      strLayout->addWidget( mEncoders.at(i), 4+i, 1 );
      mpEncodersBox->addItem( mEncoders.at(i)->name );
   }
   mpCDReader->getDevices( mpDevicesBox );
   if( !mpDevicesBox->count() )
   {
      QMessageBox::critical( 0, QApplication::applicationName(), tr("Can't find a CD or DVD drive!") );
      exit(1);
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
   tabs->addTab( strTab,               QString(tr("Stripped")) );
   tabs->addTab( mpProxyWidget,        QString(tr("Proxy")) );
   tabs->addTab( mpGlobalConfigWidget, QString(tr("Global")) );
   
   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );
   
   setLayout( mainLayout );

   connect( mpDevicesBox, SIGNAL(currentIndexChanged(const QString&)),
            mpCDReader, SLOT(setDevice(const QString&)) );
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
   i = mpDevicesBox->findText( settings.value( "Device", QString() ).toString() );
   if( i >= 0 )
   {
      mpDevicesBox->setCurrentIndex( i );
   }
   i = mpEncodersBox->findText( settings.value( "Encoder", QString() ).toString() );
   if( i < 0 )
   {
      i = 0;
   }
   mpEncodersBox->setCurrentIndex( i );
   changeEncoder( i );
   mpPattern->setText( settings.value("CreatePattern",
                       "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|").toString() );
   
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   settings.setValue( "Device", mpDevicesBox->currentText() );
   settings.setValue( "Encoder", mpEncodersBox->currentText() );
   settings.setValue( "CreatePattern", mpPattern->text() );
   
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();

   emit configChanged();
}


void ConfigDialog::changeEncoder( int id )
{
   mpCDReader->setEncoder( mEncoders.at(id) );
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->setDisabled( i != id );
   }
}


void ConfigDialog::updatePattern( const QString &text )
{
   mpPatternExample->setText( mTagList.fileName(text)+".ext" );
}
