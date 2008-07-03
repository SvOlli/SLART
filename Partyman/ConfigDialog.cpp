/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"
#include "AboutWidget.hpp"
#include "Version.hpp"

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
, mpSLARTCommunication( new QCheckBox( tr("Use SLART UDP Communication"), this ) )
, mpUDPListenerPort( new QSpinBox( this ) )
, mpNormalizeMode( new QComboBox( this ) )
, mpNormalizeValue( new QDoubleSpinBox( this ) )
, mpLogCmd( new QLineEdit( this ) )
#if 0
, mpM3uFileName( new QPushButton( this ) )
#endif
, mpGlobalSettings( new GlobalConfigWidget( this ) )
{
   setWindowTitle( tr("Partyman Settings") );
   connect( mpNormalizeMode, SIGNAL(currentIndexChanged(int)), 
            this, SLOT(handleNormalizeMode(int)) );
   connect( mpDerMixDrun, SIGNAL(clicked(bool)),
            this, SLOT(handleDerMixDrun(bool)) );
   connect( mpSLARTCommunication, SIGNAL(clicked(bool)),
            this, SLOT(handleUDPListen(bool)) );
   
   mpGlobalSettings->showClipboard();
   mpGlobalSettings->showAnimate();
   
   QStringList modes;
   modes << tr("No Normalization") 
         << tr("Normalize By Peak") 
         << tr("Normalize By Power:");
   mpNormalizeMode->addItems( modes );
   
   mpNormalizeValue->setSingleStep( 0.01 );
   mpNormalizeValue->setRange( 0.0, 1.0 );
   mpUDPListenerPort->setRange( 1, 65535 );
   mpDerMixDport->setRange( 1, 65535 );
   mpCrossfadeTime->setRange( 1, 30 );
   
   QGroupBox   *dmdGroup  = new QGroupBox( tr("DerMixD Settings:"), this );
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
   
   QGroupBox   *pmGroup  = new QGroupBox( tr("Partyman Settings:"), this );
   QGridLayout *pmLayout = new QGridLayout( pmGroup );
   pmLayout->addWidget( mpAutoConnect, 0, 0, 1, 2 );
   pmLayout->addWidget( new QLabel( tr("Crossfade Time:") ), 1, 0 );
   pmLayout->addWidget( mpCrossfadeTime, 1, 1 );
   pmLayout->addWidget( mpSLARTCommunication, 2, 0 );
   pmLayout->addWidget( mpUDPListenerPort, 2, 1 );
   pmLayout->addWidget( mpNormalizeMode, 3, 0 );
   pmLayout->addWidget( mpNormalizeValue, 3, 1 );
   pmLayout->addWidget( new QLabel( tr("External Logger:") ), 4, 0 );
   pmLayout->addWidget( mpLogCmd, 4, 1 );
#if 0
   pmLayout->addWidget( new QLabel( tr("Database File:") ), 5, 0, 1, 2 );
   pmLayout->addWidget( mpM3uFileName, 6, 0, 1, 2 );
#endif
   pmGroup->setLayout( pmLayout );
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout;
   AboutWidget *about  = new AboutWidget( this,
   tr("<table align='center'><tr><td align='center'>Our hail to the new king:</td></tr>"
   "<tr><td align='center'><a href='http://svolli.org/software/partyman/'>"
   "<img src=':/PartymanSmile.gif'></a>&nbsp;&nbsp;<a href='http://svolli.org/software/partyman/'>"
   "<img src=':/PartymanWriting.gif'></a></td></tr><tr><td align='center'>Version " SLART_VERSION
   " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart/'>SLART</a>"
   ".</td></tr><tr><td align='left'>Distributed unter the terms of the "
   "<a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>.</td></tr><tr><td align='center'>"
   "This is a frontend for <a href='http://dermixd.de/'>DerMixD</a>.</td></tr>"
   "<tr><td align='right'>Based upon the <a href='about:qt'>Qt</a> framework.</td></tr></table>") );
   
   if( QApplication::desktop()->screenGeometry().height() < 800 )
   {
      mainLayout = new QHBoxLayout( this );
      QVBoxLayout *tmpLayout1 = new QVBoxLayout;
      QVBoxLayout *tmpLayout2 = new QVBoxLayout;
      tmpLayout1->addWidget( about );
      tmpLayout1->addWidget( dmdGroup );
      tmpLayout2->addWidget( pmGroup );
      tmpLayout2->addWidget( mpGlobalSettings );
      tmpLayout1->addStretch();
      tmpLayout2->addStretch();
      tmpLayout2->addLayout( buttonLayout );
      mainLayout->addLayout( tmpLayout1 );
      mainLayout->addLayout( tmpLayout2 );
   }
   else
   {
      mainLayout = new QVBoxLayout( this );
      mainLayout->addWidget( about );
      mainLayout->addWidget( dmdGroup );
      mainLayout->addWidget( pmGroup );
      mainLayout->addWidget( mpGlobalSettings );
      mainLayout->addLayout( buttonLayout );
   }
   setLayout( mainLayout );

#if 0
   connect( mpM3uFileName, SIGNAL(clicked()),
            this, SLOT(setM3uFileName()) );
#endif
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   
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
   
   mpDerMixDhost->setText( settings.value("DerMixDhost", "localhost").toString() );
   mpDerMixDport->setValue( settings.value("DerMixDport", 8888).toInt() );
   mpDerMixDlog->setChecked( settings.value("DerMixDlog", false).toBool() );
   mpDerMixDrun->setChecked( settings.value("DerMixDrun", true).toBool() );
   mpDerMixDcmd->setText( settings.value("DerMixDcmd", "dermixd").toString() );
   mpDerMixDparams->setText( settings.value("DerMixDparams", "").toString() );
   mpAutoConnect->setChecked( settings.value("AutoConnect", false).toBool() );
   mpCrossfadeTime->setValue( settings.value("CrossfadeTime", 10).toInt() );
   mpSLARTCommunication->setChecked( settings.value("SLARTCommunication", false).toBool() );
   mpUDPListenerPort->setValue(settings.value("UDPListenerPort", 24222).toInt() );
   mpNormalizeMode->setCurrentIndex( settings.value("NormalizeMode", 0).toInt() );
   mpNormalizeValue->setValue( settings.value("NormalizeValue", 0.4).toDouble() );
   mpLogCmd->setText( settings.value("LogCmd", "").toString() );
#if 0
   mpM3uFileName->setText( settings.value("DatabaseFilename", QString() ).toString() );
#endif
   handleUDPListen( mpSLARTCommunication->isChecked() );
   handleDerMixDrun( mpDerMixDrun->isChecked() );
   mpGlobalSettings->readSettings();
   
   emit configChanged();
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
   settings.setValue( "SLARTCommunication", mpSLARTCommunication->isChecked() );
   settings.setValue( "UDPListenerPort", mpUDPListenerPort->value() );
   settings.setValue( "NormalizeMode", mpNormalizeMode->currentIndex() );
   settings.setValue( "NormalizeValue", mpNormalizeValue->value() );
   settings.setValue( "LogCmd", mpLogCmd->text() );
#if 0
   settings.setValue( "DatabaseFilename", mpM3uFileName->text().replace('\\','/') );
#endif
   mpGlobalSettings->writeSettings();

   emit configChanged();
}


#if 0
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
#endif


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
