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


UnderpassConfigDialog::UnderpassConfigDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon( ":/Underpass/Icon.png" ) );

   AboutWidget *about = new AboutWidget( this );
   mpGlobalConfigWidget->showClipboard();

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
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
   readSettings();

   WidgetShot::addWidget( "Config", this );
}


void UnderpassConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void UnderpassConfigDialog::readSettings()
{
   mpGlobalConfigWidget->readSettings();
   mpProxyWidget->readSettings();

   emit configChanged();
}


void UnderpassConfigDialog::writeSettings()
{
   mpGlobalConfigWidget->writeSettings();
   mpProxyWidget->writeSettings();

   emit configChanged();
}

