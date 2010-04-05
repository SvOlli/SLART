/**
 * src/apps/Karmadrome/ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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
#include <WidgetShot.hpp>

/* local headers */


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpNumColumnsLabel( new QLabel( tr("Number Of Columns"), this ) )
, mpNumColumns( new QSpinBox( this ) )
, mpClearBeforeImport( new QCheckBox( tr("Clear Folder Before Import"), this ) )
, mpExportAsRelative( new QCheckBox( tr("Export m3u With Relative Entries"), this ) )
, mpRandomizeExport( new QCheckBox( tr("Randomize Output Of Export m3u"), this ) )
, mpUseCheckBoxes( new QCheckBox( tr("Use Checkboxes Instead Of Buttons For Folders"), this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   setWindowIcon( QIcon(":/SLART.png") );
   
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
   
   tabs->addTab( kmdTab,               QString(tr("Karmadrome")) );
   tabs->addTab( mpGlobalConfigWidget, QString(tr("Global")) );
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
   
   WidgetShot::addWidget( "ConfigDialog", this );
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   mpNumColumns->setValue( settings.VALUE_NUMBEROFCOLUMNS );
   mpClearBeforeImport->setChecked( settings.VALUE_CLEARBEFOREIMPORT );
   mpExportAsRelative->setChecked( settings.VALUE_EXPORTASRELATIVE );
   mpRandomizeExport->setChecked( settings.VALUE_RANDOMIZEEXPORT );
   mpUseCheckBoxes->setChecked( settings.VALUE_USECHECKBOXES );
   
   mpGlobalConfigWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   settings.setValue( "NumberOfColumns",   mpNumColumns->value() );
   settings.setValue( "ClearBeforeImport", mpClearBeforeImport->isChecked() );
   settings.setValue( "ExportAsRelative",  mpExportAsRelative->isChecked() );
   settings.setValue( "RandomizeExport",   mpRandomizeExport->isChecked() );
   settings.setValue( "UseCheckBoxes",     mpUseCheckBoxes->isChecked() );
   
   mpGlobalConfigWidget->writeSettings();

   emit configChanged();
}

