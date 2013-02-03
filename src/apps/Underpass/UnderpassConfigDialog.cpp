/*
 * src/apps/Underpass/UnderpassConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "UnderpassConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QCheckBox>
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
#include "StationStorage.hpp"


UnderpassConfigDialog::UnderpassConfigDialog( StationStorage *storage,
                                              QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpStorage( storage )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mpStopOnPartyman( new QCheckBox( tr("Stop playing if Partyman is started"), this ) )
, mpRestoreDefaults( new QPushButton( tr("Restore default stations"), this ) )
, mpBufferSize( new QSpinBox( this ) )
, mRestoreDefaultsClicked( false )
{
   mpBufferSize->setRange( 50, 50000 );

   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon( ":/Underpass/Icon.png" ) );

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QWidget     *underpassTab    = new QWidget( this );
   QGridLayout *underpassLayout = new QGridLayout( underpassTab );
   underpassLayout->addWidget( new QLabel( tr("Buffer Size:"), this ), 0, 0 );
   underpassLayout->addWidget( mpBufferSize,                           0, 1 );
   underpassLayout->addWidget( mpStopOnPartyman,                       1, 0, 1, 2 );
   underpassLayout->addWidget( mpRestoreDefaults,                      8, 0, 1, 2 );
   underpassLayout->setColumnStretch( 0, 1 );
   underpassLayout->setRowStretch( 7, 1 );
   underpassTab->setLayout( underpassLayout );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( underpassTab,            tr("Underpass") );
   tabs->addTab( mpProxyWidget,           tr("Proxy") );
   tabs->addTab( mpGlobalConfigWidget,    tr("Global") );

   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );
   setLayout( mainLayout );

   connect( mpRestoreDefaults, SIGNAL(clicked()),
            this, SLOT(restoreDefaultsClicked()) );
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   readSettings();

   WidgetShot::addWidget( "Config", this );
}


void UnderpassConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void UnderpassConfigDialog::restoreDefaultsClicked()
{
   mRestoreDefaultsClicked = true;
}


void UnderpassConfigDialog::readSettings()
{
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();

   mpBufferSize->setValue( Settings::value( Settings::UnderpassBufferSize ) );
   mpStopOnPartyman->setChecked( Settings::value( Settings::UnderpassStopOnPartymanStart ));

   mRestoreDefaultsClicked = false;
   emit configChanged();
}


void UnderpassConfigDialog::writeSettings()
{
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();

   Settings::setValue( Settings::UnderpassBufferSize, mpBufferSize->value() );
   Settings::setValue( Settings::UnderpassStopOnPartymanStart, mpStopOnPartyman->isChecked() );

   if( mRestoreDefaultsClicked )
   {
      mpStorage->createDefaults();
   }
   emit configChanged();
}
