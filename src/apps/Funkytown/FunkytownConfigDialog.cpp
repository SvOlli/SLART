/*
 * src/apps/Funkytown/FunkytownConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "FunkytownConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTextBrowser>

/* local library headers */
#include <AboutWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <ProxyWidget.hpp>
#include <Settings.hpp>

/* local headers */


FunkytownConfigDialog::FunkytownConfigDialog( QWidget *parent )
: QDialog( parent )
, mpProxyWidget( new ProxyWidget( this ) )
, mpLogList( new QListWidget( this ) )
, mpHelpText( new QTextBrowser( this ) )
, mpGlobalConfigWidget( new GlobalConfigWidget( this ) )
, mpOverwrite( new QCheckBox( tr("Overwrite Files During Download"), this ) )
, mpCoverArt( new QCheckBox( tr("Download Cover-Like Artwork"), this ) )
, mpTollKeep( new QCheckBox( tr("Count Downloaded Files And Bytes"), this ) )
, mpDownloadedFiles( new QLabel( this ) )
, mpDownloadedBytes( new QLabel( this ) )
, mpClearButton( new QPushButton( tr("Clear"), this ) )
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon(":/SLART.png") );

   mpHelpText->setReadOnly( true );
   mpHelpText->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   mpHelpText->setOpenExternalLinks( true );
   mpHelpText->setSource( QUrl("qrc:/Usage.html") );

   AboutWidget *about( new AboutWidget( this ) );
   QPushButton *okButton( new QPushButton(tr("OK"), this) );
   QPushButton *cancelButton( new QPushButton(tr("Cancel"), this) );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QWidget     *settingsTab    = new QWidget( this );
   QGridLayout *settingsLayout = new QGridLayout( settingsTab );
   settingsLayout->addWidget( mpOverwrite, 0, 0, 1, 3 );
   settingsLayout->addWidget( mpCoverArt,  1, 0, 1, 3 );
   settingsLayout->addWidget( mpTollKeep,  2, 0, 1, 3 );
   settingsLayout->addWidget( new QLabel( tr("Downloaded Files:"), this ), 3, 0 );
   settingsLayout->addWidget( mpDownloadedFiles, 3, 1 );
   settingsLayout->addWidget( new QLabel( tr("Downloaded Bytes:"), this ), 4, 0 );
   settingsLayout->addWidget( mpDownloadedBytes, 4, 1 );
   settingsLayout->addWidget( mpClearButton, 3, 2, 2, 1 );
   settingsLayout->setRowStretch( 5, 1 );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( mpHelpText,           tr("Help") );
   tabs->addTab( settingsTab,          tr("Funkytown") );
   tabs->addTab( mpProxyWidget,        tr("Proxy") );
   tabs->addTab( mpGlobalConfigWidget, tr("Global") );
   tabs->addTab( mpLogList,            tr("Log") );

   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );

   setLayout( mainLayout );

   connect( mpClearButton, SIGNAL(clicked()),
            this, SLOT(handleClear()) );
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


void FunkytownConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void FunkytownConfigDialog::logMessage( const QString &message )
{
   mpLogList->addItem( message );
   mpLogList->scrollToBottom();
}


void FunkytownConfigDialog::readSettings()
{
   mpProxyWidget->readSettings();

   mpOverwrite->setChecked( Settings::value( Settings::FunkytownOverwrite ) );
   mpCoverArt->setChecked( Settings::value( Settings::FunkytownCoverArt ) );
   mpTollKeep->setChecked( Settings::value( Settings::FunkytownTollKeep ) );
   mpDownloadedFiles->setText( QString::number( Settings::value( Settings::FunkytownFiles ) ) );
   mpDownloadedBytes->setText( QString::number( Settings::value( Settings::FunkytownBytes ) / 1048576 ) + "M" );
}


void FunkytownConfigDialog::writeSettings()
{
   mpProxyWidget->writeSettings();

   QString userAgent( Settings::value( Settings::FunkytownUserAgent ) );
   if( userAgent.isEmpty() )
   {
      userAgent = "Shockwave Flash";
      Settings::setValue( Settings::FunkytownUserAgent, userAgent );
   }
   Settings::setValue( Settings::FunkytownOverwrite, mpOverwrite->isChecked() );
   Settings::setValue( Settings::FunkytownCoverArt, mpCoverArt->isChecked() );
   Settings::setValue( Settings::FunkytownTollKeep, mpTollKeep->isChecked() );
}


void FunkytownConfigDialog::handleClear()
{
   if( QMessageBox::question( this,
         QApplication::applicationName() + ": " + tr("Clear Statistics"),
         tr("Do you really want to clear the statistics?"),
         QMessageBox::Ok | QMessageBox::Cancel,
         QMessageBox::Cancel ) == QMessageBox::Ok )
   {
      QSettings *settings = Settings::get();
      settings->remove( "Files" );
      settings->remove( "Bytes" );
   }
}
