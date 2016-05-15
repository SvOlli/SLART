/*
 * src/apps/Karmadrome/KarmadromeConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "KarmadromeConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */


KarmadromeConfigDialog::KarmadromeConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpNumColumnsLabel( new QLabel( tr("Number Of Columns"), this ) )
, mpNumColumns( new QSpinBox( this ) )
, mpClearBeforeImport( new QCheckBox( tr("Clear Group Before Import"), this ) )
, mpExportAsRelative( new QCheckBox( tr("Export m3u With Relative Entries"), this ) )
, mpRandomizeExport( new QCheckBox( tr("Randomize Output Of Export m3u"), this ) )
, mpUseCheckBoxes( new QCheckBox( tr("Use Checkboxes Instead Of Buttons For Groups"), this ) )
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon( ":/Karmadrome/Icon.png" ) );

   mpNumColumns->setRange( 1, 9 );

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();

   QWidget     *kmdTab    = new QWidget( this );
   QGridLayout *kmdLayout = new QGridLayout( kmdTab );
   kmdLayout->addWidget( mpNumColumnsLabel,   0, 0 );
   kmdLayout->addWidget( mpNumColumns,        0, 1 );
   kmdLayout->addWidget( mpClearBeforeImport, 1, 0, 1, 2 );
   kmdLayout->addWidget( mpExportAsRelative,  2, 0, 1, 2 );
   kmdLayout->addWidget( mpRandomizeExport,   3, 0, 1, 2 );
   kmdLayout->addWidget( mpUseCheckBoxes,     4, 0, 1, 2 );
   kmdLayout->setRowStretch( 5, 1 );
   kmdTab->setLayout( kmdLayout );

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );

   tabs->addTab( kmdTab,               tr("Karmadrome") );
   tabs->addTab( mpGlobalConfigWidget, tr("Global") );
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

   readSettings();

   WidgetShot::addWidget( "Config", this );
}


KarmadromeConfigDialog::~KarmadromeConfigDialog()
{
}


int KarmadromeConfigDialog::exec()
{
   readSettings();
   return QDialog::exec();
}


void KarmadromeConfigDialog::readSettings()
{
   mpNumColumns->setValue( Settings::value( Settings::KarmadromeNumberOfColumns ) );
   mpClearBeforeImport->setChecked( Settings::value( Settings::KarmadromeClearBeforeImport ) );
   mpExportAsRelative->setChecked( Settings::value( Settings::KarmadromeExportAsRelative ) );
   mpRandomizeExport->setChecked( Settings::value( Settings::KarmadromeRandomizeExport ) );
   mpUseCheckBoxes->setChecked( Settings::value( Settings::KarmadromeUseCheckBoxes ) );

   mpGlobalConfigWidget->readSettings();

   emit configChanged();
}


void KarmadromeConfigDialog::writeSettings()
{
   Settings::setValue( Settings::KarmadromeNumberOfColumns, mpNumColumns->value() );
   Settings::setValue( Settings::KarmadromeClearBeforeImport, mpClearBeforeImport->isChecked() );
   Settings::setValue( Settings::KarmadromeExportAsRelative, mpExportAsRelative->isChecked() );
   Settings::setValue( Settings::KarmadromeRandomizeExport, mpRandomizeExport->isChecked() );
   Settings::setValue( Settings::KarmadromeUseCheckBoxes, mpUseCheckBoxes->isChecked() );

   mpGlobalConfigWidget->writeSettings();

   emit configChanged();
}

