/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "MySettings.hpp"
#include "GlobalConfigWidget.hpp"
#include "AboutWidget.hpp"


#include <QtGui>
#include "SLARTCom.hpp"


ConfigDialog::ConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpNumColumnsLabel( new QLabel( tr("Number Of Columns"), this ) )
, mpNumColumns( new QSpinBox( this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   
   mpNumColumns->setRange( 1, 9 );
   
   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();
   
   QGroupBox   *kmdGroup  = new QGroupBox( tr("Karmadrome Settings:"), this );
   QGridLayout *kmdLayout = new QGridLayout( kmdGroup );
   kmdLayout->addWidget( mpNumColumnsLabel, 0, 0 );
   kmdLayout->addWidget( mpNumColumns, 0, 1 );
   kmdGroup->setLayout( kmdLayout );
   
   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   
   mainLayout->addWidget( kmdGroup );
   mainLayout->addWidget( mpGlobalConfigWidget );
   mainLayout->addWidget( about );
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
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   mpNumColumns->setValue( settings.value("NumberOfColumns", 3).toInt() );
   
   mpGlobalConfigWidget->readSettings();
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;
   settings.setValue( "NumberOfColumns", mpNumColumns->value() );
   
   mpGlobalConfigWidget->writeSettings();

   emit configChanged();
}

