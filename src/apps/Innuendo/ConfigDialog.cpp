/**
 * src/apps/Innuendo/ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "ConfigDialog.hpp"

#include <QtGui>

#include "AboutWidget.hpp"
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"
#include "WidgetShot.hpp"
#include "Satellite.hpp"
#include "SatelliteConfigWidget.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpSatelliteConfigWidget( new SatelliteConfigWidget( this ) )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mpBufferSize( new QSpinBox( this ) )
, mpAutostartPartyman( new QPushButton( tr("Partyman"), this) )
, mpAutostartKarmadrome( new QPushButton( tr("Karmadrome"), this) )
, mpAutostartRubberbandman( new QPushButton( tr("Rubberbandman"), this) )
, mpAutostartStripped( new QPushButton( tr("Stripped"), this) )
, mpAutostartFunkytown( new QPushButton( tr("Funkytown"), this) )
, mpAutostartCreep( new QPushButton( tr("Creep"), this) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   setWindowIcon( QIcon(":/SLART.png") );
   
   mpAutostartPartyman->setCheckable( true );
   mpAutostartKarmadrome->setCheckable( true );
   mpAutostartRubberbandman->setCheckable( true );
   mpAutostartStripped->setCheckable( true );
   mpAutostartFunkytown->setCheckable( true );
   mpAutostartCreep->setCheckable( true );
   
   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );
   
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );
   
   QGroupBox   *autostartBox    = new QGroupBox( tr("Autostart With Innuendo:") );
   QGridLayout *autostartLayout = new QGridLayout( autostartBox );
   autostartLayout->addWidget( mpAutostartPartyman, 0, 0 );
   autostartLayout->addWidget( mpAutostartKarmadrome, 0, 1 );
   autostartLayout->addWidget( mpAutostartRubberbandman, 0, 2 );
   autostartLayout->addWidget( mpAutostartStripped, 1, 0 );
   autostartLayout->addWidget( mpAutostartFunkytown, 1, 1 );
   autostartLayout->addWidget( mpAutostartCreep, 1, 2 );
   autostartBox->setLayout( autostartLayout );
   
   QWidget     *iTab    = new QWidget( this );
   QGridLayout *iLayout = new QGridLayout( iTab );
   iLayout->addWidget( new QLabel( tr("Buffer Size:"), this ), 0, 0 );
   iLayout->addWidget( mpBufferSize,                           0, 1 );
   iLayout->addWidget( autostartBox,                           2, 0, 1, 2 );
   iLayout->setColumnStretch( 0, 1 );
   iLayout->setRowStretch( 1, 1 );
   mpBufferSize->setRange( 50, 50000 );
   
   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( mpSatelliteConfigWidget, QString(tr("Satellite")) );
   tabs->addTab( iTab,                    QString(tr("Innuendo")) );
   tabs->addTab( mpProxyWidget,           QString(tr("Proxy")) );
   tabs->addTab( mpGlobalConfigWidget,    QString(tr("Global")) );
   
   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );
   setLayout( mainLayout );
   
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   connect( mpSatelliteConfigWidget, SIGNAL(useInnuendoClicked(bool)),
            mpGlobalConfigWidget, SLOT(setSatelliteClicked(bool)) );
   connect( mpGlobalConfigWidget, SIGNAL(useSatelliteClicked(bool)),
            mpSatelliteConfigWidget, SLOT(setInnuendoClicked(bool)) );
   readSettings();
   
   WidgetShot::addWidget( "ConfigDialog", this );
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   mpSatelliteConfigWidget->readSettings();
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();
   
   MySettings settings;
   mpBufferSize->setValue( settings.VALUE_BUFFERSIZE );
   QStringList autostart( settings.VALUE_STARTUP );
   
   mpAutostartPartyman->setChecked( autostart.contains( "Partyman" ) );
   mpAutostartKarmadrome->setChecked( autostart.contains( "Karmadrome" ) );
   mpAutostartRubberbandman->setChecked( autostart.contains( "Rubberbandman" ) );
   mpAutostartStripped->setChecked( autostart.contains( "Stripped" ) );
   mpAutostartFunkytown->setChecked( autostart.contains( "Funkytown" ) );
   mpAutostartCreep->setChecked( autostart.contains( "Creep" ) );
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   mpSatelliteConfigWidget->writeSettings();
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();
   
   MySettings settings;
   QStringList autostart;
   if( mpAutostartPartyman->isChecked() ) { autostart.append( "Partyman" ); }
   if( mpAutostartKarmadrome->isChecked() ) { autostart.append( "Karmadrome" ); }
   if( mpAutostartRubberbandman->isChecked() ) { autostart.append( "Rubberbandman" ); }
   if( mpAutostartStripped->isChecked() ) { autostart.append( "Stripped" ); }
   if( mpAutostartFunkytown->isChecked() ) { autostart.append( "Funkytown" ); }
   if( mpAutostartCreep->isChecked() ) { autostart.append( "Creep" ); }
   settings.setValue( "BufferSize", mpBufferSize->value() );
   if( autostart.isEmpty() )
   {
      settings.remove( "Startup" );
   }
   else
   {
      settings.setValue( "Startup", autostart );
   }
   
   emit configChanged();
}

