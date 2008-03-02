/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "MyClipboard.hpp"

#include <QtGui>


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpDerMixDhostLabel( new QLabel( tr("Hostname:") ) )
, mpDerMixDhost( new QLineEdit( this ) )
, mpDerMixDportLabel( new QLabel( tr("Port:") ) )
, mpDerMixDport( new QSpinBox( this ) )
, mpDerMixDlog( new QCheckBox( tr("Log communication to stdout"), this ) )
, mpDerMixDrun( new QCheckBox( tr("Start/Stop via Partyman"), this ) )
, mpDerMixDcmdLabel( new QLabel( tr("Command:"), this ) )
, mpDerMixDcmd( new QLineEdit( this ) )
, mpDerMixDparamsLabel( new QLabel( tr("Parameters:"), this ) )
, mpDerMixDparams( new QLineEdit( this ) )
, mpAutoConnect( new QCheckBox( tr("Connect On Startup"), this ) )
, mpCrossfadeTime( new QSpinBox( this ) )
, mpSLATCommunication( new QCheckBox( tr("Use SLAT UDP Communication"), this ) )
, mpUDPListenerPort( new QSpinBox( this ) )
, mpNormalizeMode( new QComboBox( this ) )
, mpNormalizeValue( new QDoubleSpinBox( this ) )
, mpClipboard( new MyClipboard( this, false ) )
, mpLogCmd( new QLineEdit( this ) )
, mpM3uFileName( new QPushButton( this ) )
{
   setWindowTitle( tr("Partyman Settings") );
   connect( mpNormalizeMode, SIGNAL(currentIndexChanged(int)), 
            this, SLOT(handleNormalizeMode(int)) );
   connect( mpDerMixDrun, SIGNAL(clicked(bool)),
            this, SLOT(handleDerMixDrun(bool)) );
   connect( mpSLATCommunication, SIGNAL(clicked(bool)),
            this, SLOT(handleUDPListen(bool)) );
   
   QStringList modes;
   modes << tr("No Normalization") 
         << tr("Normalize By Peak") 
         << tr("Normalize By Power:");
   mpNormalizeMode->addItems( modes );
   
   mpNormalizeValue->setSingleStep( 0.01 );
   mpNormalizeValue->setRange( 0.0, 1.0 );
   mpUDPListenerPort->setRange( 1, 65535 );
   mpDerMixDport->setRange( 1, 65535 );
   mpCrossfadeTime->setRange( 0, 30 );
   readSettings();
   
   QGroupBox   *dmdGroup  = new QGroupBox( tr("DerMixD Parameters:"), this );
   QGridLayout *dmdLayout = new QGridLayout( dmdGroup );
   dmdLayout->addWidget( mpDerMixDrun, 0, 0, 1, 2 );
   dmdLayout->addWidget( mpDerMixDcmdLabel, 1, 0 );
   dmdLayout->addWidget( mpDerMixDcmd, 1, 1 );
   dmdLayout->addWidget( mpDerMixDparamsLabel, 2, 0 );
   dmdLayout->addWidget( mpDerMixDparams, 2, 1 );
   dmdLayout->addWidget( mpDerMixDhostLabel, 3, 0 );
   dmdLayout->addWidget( mpDerMixDhost, 3, 1 );
   dmdLayout->addWidget( mpDerMixDportLabel, 4, 0 );
   dmdLayout->addWidget( mpDerMixDport, 4, 1 );
   dmdLayout->addWidget( mpDerMixDlog, 5, 0, 1, 2 );
   dmdGroup->setLayout( dmdLayout );
   
   QGroupBox   *pmGroup  = new QGroupBox( tr("Partyman Parameters:"), this );
   QGridLayout *pmLayout = new QGridLayout( pmGroup );
   pmLayout->addWidget( mpAutoConnect, 0, 0, 1, 2 );
   pmLayout->addWidget( new QLabel( tr("Crossfade Time:") ), 1, 0 );
   pmLayout->addWidget( mpCrossfadeTime, 1, 1 );
   pmLayout->addWidget( mpSLATCommunication, 2, 0 );
   pmLayout->addWidget( mpUDPListenerPort, 2, 1 );
   pmLayout->addWidget( mpNormalizeMode, 3, 0 );
   pmLayout->addWidget( mpNormalizeValue, 3, 1 );
   pmLayout->addWidget( mpClipboard->label(), 4, 0 );
   pmLayout->addWidget( mpClipboard->comboBox(), 4, 1 );
   pmLayout->addWidget( new QLabel( tr("External Logger:") ), 5, 0 );
   pmLayout->addWidget( mpLogCmd, 5, 1 );
   pmLayout->addWidget( new QLabel( tr("Database File:") ), 6, 0, 1, 2 );
   pmLayout->addWidget( mpM3uFileName, 7, 0, 1, 2 );
   pmGroup->setLayout( pmLayout );
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QVBoxLayout *mainLayout = new QVBoxLayout;
   
   if( QApplication::desktop()->screenGeometry().height() < 600 )
   {
      QHBoxLayout *interimLayout = new QHBoxLayout;
      interimLayout->addWidget( dmdGroup );
      interimLayout->addWidget( pmGroup );
      mainLayout->addLayout( interimLayout );
   }
   else
   {
      mainLayout->addWidget( dmdGroup );
      mainLayout->addWidget( pmGroup );
   }
   mainLayout->addLayout( buttonLayout );
   setLayout( mainLayout );

   connect( mpM3uFileName, SIGNAL(clicked()), this, SLOT(setM3uFileName()) );
   connect( okButton,      SIGNAL(clicked()), this, SLOT(accept()) );
   connect( cancelButton,  SIGNAL(clicked()), this, SLOT(reject()) );
   connect( this,         SIGNAL(accepted()), this, SLOT(writeSettings()) );
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   
   mpDerMixDhost->setText( settings.value("DerMixDhost", "localhost").toString() );
   mpDerMixDport->setValue( settings.value("DerMixDport", 8888).toInt() );
   mpDerMixDlog->setChecked( settings.value("DerMixDlog", false).toBool() );
   mpDerMixDrun->setChecked( settings.value("DerMixDrun", true).toBool() );
   mpDerMixDcmd->setText( settings.value("DerMixDcmd", "dermixd").toString() );
   mpDerMixDparams->setText( settings.value("DerMixDparams", "").toString() );
   mpAutoConnect->setChecked( settings.value("AutoConnect", false).toBool() );
   mpCrossfadeTime->setValue( settings.value("CrossfadeTime", 10).toInt() );
   mpSLATCommunication->setChecked( settings.value("SLATCommunication", false).toBool() );
   mpUDPListenerPort->setValue(settings.value("UDPListenerPort", 24222).toInt() );
   mpNormalizeMode->setCurrentIndex( settings.value("NormalizeMode", 0).toInt() );
   mpNormalizeValue->setValue( settings.value("NormalizeValue", 0.4).toDouble() );
   mpLogCmd->setText( settings.value("LogCmd", "").toString() );
   mpM3uFileName->setText( settings.value("DatabaseFilename", QString() ).toString() );
   mpClipboard->readSettings();
   handleUDPListen( mpSLATCommunication->isChecked() );
   handleDerMixDrun( mpDerMixDrun->isChecked() );
   
   emit configChanged();
}


void ConfigDialog::handleConnected( bool connected )
{
   mpDerMixDhostLabel->setDisabled( connected );
   mpDerMixDhost->setDisabled( connected );
   mpDerMixDportLabel->setDisabled( connected );
   mpDerMixDport->setDisabled( connected );
   mpDerMixDrun->setDisabled( connected );
   mpDerMixDcmdLabel->setDisabled( connected );
   mpDerMixDcmd->setDisabled( connected );
   mpDerMixDparamsLabel->setDisabled( connected );
   mpDerMixDparams->setDisabled( connected );
   if( !connected )
   {
      handleDerMixDrun( mpDerMixDrun->isChecked() );
   }
}


void ConfigDialog::writeSettings()
{
   MySettings settings;

   settings.setValue( "DerMixDhost", mpDerMixDhost->text() );
   settings.setValue( "DerMixDport", mpDerMixDport->value() );
   settings.setValue( "DerMixDlog", mpDerMixDlog->isChecked() );
   settings.setValue( "DerMixDrun", mpDerMixDrun->isChecked() );
   settings.setValue( "DerMixDcmd", mpDerMixDcmd->text() );
   settings.setValue( "DerMixDparams", mpDerMixDparams->text() );
   settings.setValue( "AutoConnect", mpAutoConnect->isChecked() );
   settings.setValue( "CrossfadeTime", mpCrossfadeTime->value() );
   settings.setValue( "SLATCommunication", mpSLATCommunication->isChecked() );
   settings.setValue( "UDPListenerPort", mpUDPListenerPort->value() );
   settings.setValue( "NormalizeMode", mpNormalizeMode->currentIndex() );
   settings.setValue( "NormalizeValue", mpNormalizeValue->value() );
   settings.setValue( "LogCmd", mpLogCmd->text() );
   settings.setValue( "DatabaseFilename", mpM3uFileName->text().replace('\\','/') );
   mpClipboard->writeSettings();

   emit configChanged();
}


void ConfigDialog::setM3uFileName()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::ExistingFile );
   fileDialog.setDirectory( QDir( mpM3uFileName->text() ).absolutePath() );
   fileDialog.setFilter( "Playlist used as Database (*.m3u)" );
   fileDialog.setReadOnly( true );

   if( fileDialog.exec() )
   {
      MySettings settings;
      QString result( fileDialog.selectedFiles().at(0) );
      mpM3uFileName->setText( result );
   }
}


void ConfigDialog::handleDerMixDrun( bool checked )
{
   mpDerMixDcmdLabel->setDisabled( !checked );
   mpDerMixDcmd->setDisabled( !checked );
   mpDerMixDparamsLabel->setDisabled( !checked );
   mpDerMixDparams->setDisabled( !checked );
   mpDerMixDhostLabel->setDisabled( checked );
   mpDerMixDhost->setDisabled( checked );
}


void ConfigDialog::handleUDPListen( bool checked )
{
   mpUDPListenerPort->setDisabled( !checked );
}


void ConfigDialog::handleNormalizeMode( int mode )
{
   mpNormalizeValue->setDisabled( mode != 2 );
}
