/*
 * src/apps/Notorious/NotoriousConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "NotoriousConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

/* local library headers */
#include <AboutWidget.hpp>
#include <ProxyWidget.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "FreeDBImport.hpp"


NotoriousConfigDialog::NotoriousConfigDialog( QWidget *parent )
: QDialog( parent )
, mpFreeDBImport( new FreeDBImport( this ) )
, mpLogList( new QListWidget( this ) )
, mpImportFile( new QLineEdit( this ) )
, mpCount( new QLabel( this ) )
, mpFileName( new QLabel( this ) )
, mpTimeSpent( new QLabel( this ) )
, mpImportButton( new QPushButton( tr("Run Import"), this ) )
, mTimeSpent()
{
   setWindowTitle( QCoreApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon( ":/Notorious/Icon.png" ) );

   AboutWidget *about( new AboutWidget( this ) );
   QLabel      *freedbInfo = new QLabel( tr("FreeDB database archives for import can be downloaded at "
                                            "<a href='http://ftp.freedb.org/pub/freedb/'>freedb.org</a>."), this );
   freedbInfo->setOpenExternalLinks( true );
   QPushButton *okButton( new QPushButton(tr("OK"), this) );
   QPushButton *cancelButton( new QPushButton(tr("Cancel"), this) );
   QPushButton *browseButton = new QPushButton( "...", this );
   /* evil hack */
   browseButton->setMaximumWidth( browseButton->height() );
   mpImportFile->setText( "/media/share/freedb/" );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QWidget     *importTab    = new QWidget( this );
   QGridLayout *importLayout = new QGridLayout( importTab );
   importLayout->addWidget( freedbInfo, 0, 0, 1, 3 );
   importLayout->addWidget( mpImportFile, 1, 0, 1, 2 );
   importLayout->addWidget( browseButton, 1, 2 );
   importLayout->addWidget( new QLabel( tr("Files Processed:"), this ), 2, 0 );
   importLayout->addWidget( new QLabel( tr("File Name:"), this ),       3, 0 );
   importLayout->addWidget( new QLabel( tr("Time Spent:"), this ),      4, 0 );
   importLayout->addWidget( mpCount, 2, 1, 1, 2 );
   importLayout->addWidget( mpFileName, 3, 1, 1, 2 );
   importLayout->addWidget( mpTimeSpent, 4, 1, 1, 2 );
   importLayout->addWidget( mpImportButton, 6, 0, 1, 3 );
   importLayout->setColumnStretch( 1, 1 );
   importLayout->setRowStretch( 5, 1 );

   QBoxLayout *mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs       = new QTabWidget( this );
   tabs->addTab( importTab, tr("Import") );
   tabs->addTab( mpLogList, tr("Log") );

   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );

   setLayout( mainLayout );

   connect( mpFreeDBImport, SIGNAL(processed(unsigned,const char*)),
            this, SLOT(handleProgress(unsigned,const char*)) );
   connect( browseButton, SIGNAL(clicked()),
            this, SLOT(setFileName()) );
   connect( mpImportFile, SIGNAL(textChanged(QString)),
            this, SLOT(checkValidFile(QString)) );
   connect( mpImportButton, SIGNAL(clicked()),
            this, SLOT(handleImport()) );
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );

   readSettings();
   checkValidFile( QString() );

   WidgetShot::addWidget( "Config", this );
}


void NotoriousConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void NotoriousConfigDialog::logMessage( const QString &message )
{
   mpLogList->addItem( message );
   mpLogList->scrollToBottom();
}


void NotoriousConfigDialog::handleProgress( unsigned count, const char *filename )
{
   mpCount->setText( QString::number( count ) );
   mpFileName->setText( QString(filename) );
   mpTimeSpent->setText( QTime(0,0).addMSecs( mTimeSpent.elapsed() ).toString("hh:mm:ss") );
}


void NotoriousConfigDialog::readSettings()
{
}


void NotoriousConfigDialog::writeSettings()
{
}


void NotoriousConfigDialog::setFileName()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::ExistingFile );
   fileDialog.setDirectory( mpImportFile->text() );
   fileDialog.setNameFilter("FreeDB Dumps (*.tar.bz2)");
   fileDialog.setReadOnly( true );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      mpImportFile->setText( result );
   }
}


void NotoriousConfigDialog::checkValidFile( const QString &fileName )
{
   QFileInfo qfi( fileName );
   if( qfi.isFile() && qfi.isReadable() && fileName.endsWith( ".tar.bz2", Qt::CaseInsensitive ) )
   {
      mpImportButton->setDisabled( false );
   }
   else
   {
      mpImportButton->setDisabled( true );
   }
}


void NotoriousConfigDialog::handleImport()
{
   if( mpFreeDBImport->isRunning() )
   {
      mpFreeDBImport->stop();
      mpImportButton->setText( tr("Run Import") );
   }
   else
   {
      mpFreeDBImport->setFileName( mpImportFile->text() );
      mpFreeDBImport->start();
      mpImportButton->setText( tr("Cancel Import") );
      mTimeSpent.start();
   }
}
