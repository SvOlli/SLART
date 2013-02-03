/*
 * src/apps/Innuendo/InnuendoConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "InnuendoConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <Settings.hpp>
#include <ProxyWidget.hpp>
#include <Satellite.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "SatelliteConfigWidget.hpp"


InnuendoConfigDialog::InnuendoConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpSatelliteConfigWidget( new SatelliteConfigWidget( this ) )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mpBufferSize( new QSpinBox( this ) )
, mpAutostartPartyman( new QPushButton( QIcon(":/Partyman/Icon.png"), tr("Partyman"), this) )
, mpAutostartKarmadrome( new QPushButton( QIcon(":/Karmadrome/Icon.png"), tr("Karmadrome"), this) )
, mpAutostartRubberbandman( new QPushButton( QIcon(":/Rubberbandman/Icon.png"), tr("Rubberbandman"), this) )
, mpAutostartStripped( new QPushButton( QIcon(":/Stripped/Icon.png"), tr("Stripped"), this) )
, mpAutostartFunkytown( new QPushButton( QIcon(":/Funkytown/Icon.png"), tr("Funkytown"), this) )
, mpAutostartNotorious( new QPushButton( QIcon(":/Notorious/Icon.png"), tr("Notorious"), this) )
, mpAutostartCreep( new QPushButton( QIcon(":/Creep/Icon.png"), tr("Creep"), this) )
, mpAutostartUnderpass( new QPushButton( QIcon(":/Underpass/Icon.png"), tr("Underpass"), this) )
{
   mpBufferSize->setRange( 50, 50000 );

   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon( ":/Innuendo/Icon.png" ) );

   mpAutostartPartyman->setCheckable( true );
   mpAutostartKarmadrome->setCheckable( true );
   mpAutostartRubberbandman->setCheckable( true );
   mpAutostartStripped->setCheckable( true );
   mpAutostartFunkytown->setCheckable( true );
   mpAutostartCreep->setCheckable( true );
   mpAutostartUnderpass->setCheckable( true );

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QGroupBox   *autostartBox    = new QGroupBox( tr("Autostart With Innuendo:") );
   QGridLayout *autostartLayout = new QGridLayout( autostartBox );
   autostartLayout->addWidget( mpAutostartPartyman,      0, 0 );
   autostartLayout->addWidget( mpAutostartKarmadrome,    0, 1 );
   autostartLayout->addWidget( mpAutostartRubberbandman, 0, 2 );
   autostartLayout->addWidget( mpAutostartStripped,      1, 0 );
   autostartLayout->addWidget( mpAutostartFunkytown,     1, 1 );
   autostartLayout->addWidget( mpAutostartNotorious,     1, 2 );
   autostartLayout->addWidget( mpAutostartCreep,         2, 0 );
   autostartLayout->addWidget( mpAutostartUnderpass,     2, 1 );
   autostartBox->setLayout( autostartLayout );

   QWidget     *iTab    = new QWidget( this );
   QGridLayout *iLayout = new QGridLayout( iTab );
   iLayout->addWidget( new QLabel( tr("Buffer Size:"), this ), 0, 0 );
   iLayout->addWidget( mpBufferSize,                           0, 1 );
   iLayout->addWidget( autostartBox,                           2, 0, 1, 2 );
   iLayout->setColumnStretch( 0, 1 );
   iLayout->setRowStretch( 1, 1 );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( mpSatelliteConfigWidget, tr("Satellite") );
   tabs->addTab( iTab,                    tr("Innuendo") );
   tabs->addTab( mpProxyWidget,           tr("Proxy") );
   tabs->addTab( mpGlobalConfigWidget,    tr("Global") );

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

   WidgetShot::addWidget( "Config", this );
}


void InnuendoConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void InnuendoConfigDialog::readSettings()
{
   mpSatelliteConfigWidget->readSettings();
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();

   mpBufferSize->setValue( Settings::value( Settings::InnuendoBufferSize ) );
   QStringList autostart( Settings::value( Settings::InnuendoStartup ) );

   mpAutostartPartyman->setChecked( autostart.contains( "Partyman" ) );
   mpAutostartKarmadrome->setChecked( autostart.contains( "Karmadrome" ) );
   mpAutostartRubberbandman->setChecked( autostart.contains( "Rubberbandman" ) );
   mpAutostartStripped->setChecked( autostart.contains( "Stripped" ) );
   mpAutostartFunkytown->setChecked( autostart.contains( "Funkytown" ) );
   mpAutostartNotorious->setChecked( autostart.contains( "Notorious" ) );
   mpAutostartCreep->setChecked( autostart.contains( "Creep" ) );
   mpAutostartUnderpass->setChecked( autostart.contains( "Underpass" ) );

   emit configChanged();
}


void InnuendoConfigDialog::writeSettings()
{
   mpSatelliteConfigWidget->writeSettings();
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();

   QStringList autostart;
   if( mpAutostartPartyman->isChecked() ) { autostart.append( "Partyman" ); }
   if( mpAutostartKarmadrome->isChecked() ) { autostart.append( "Karmadrome" ); }
   if( mpAutostartRubberbandman->isChecked() ) { autostart.append( "Rubberbandman" ); }
   if( mpAutostartStripped->isChecked() ) { autostart.append( "Stripped" ); }
   if( mpAutostartFunkytown->isChecked() ) { autostart.append( "Funkytown" ); }
   if( mpAutostartNotorious->isChecked() ) { autostart.append( "Notorious" ); }
   if( mpAutostartCreep->isChecked() ) { autostart.append( "Creep" ); }
   if( mpAutostartUnderpass->isChecked() ) { autostart.append( "Underpass" ); }
   Settings::setValue( Settings::InnuendoBufferSize, mpBufferSize->value() );
   Settings::setValue( Settings::InnuendoStartup, autostart );

   emit configChanged();
}

