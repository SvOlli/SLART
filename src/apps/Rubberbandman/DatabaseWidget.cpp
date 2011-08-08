/**
 * src/apps/Rubberbandman/DatabaseWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DatabaseWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QString>
#include <QSqlTableModel>
#include <QTableView>

/* local library headers */
#include <Database.hpp>
#include <MySettings.hpp>

/* local headers */
#include "DatabaseWorker.hpp"
#include "ConfigDialog.hpp"


DatabaseWidget::DatabaseWidget( QWidget *parent )
: QWidget( parent )
, mpDatabaseWorker( new DatabaseWorker() )
, mpBaseDir( new QLineEdit( this ) )
, mpUpdateButton( new QPushButton( tr("Update"), this ) )
, mpCleanupButton( new QPushButton( tr("Clean Up"), this ) )
, mpImportButton( new QPushButton( tr("Import m3u"), this ) )
, mpMessage( new QLabel( this ) )
, mpPartymanInfo( new QLabel( this ) )
, mPartymanLocal( false )
, mCheckedText()
, mProcessedText()
{
   QPushButton *browseButton  = new QPushButton( "...", this );
   /* evil hack */
   browseButton->setMaximumWidth( browseButton->height() );

   QCompleter *completer = new QCompleter( this );
   completer->setModel( new QDirModel( QStringList(),
                                       QDir::NoDotAndDotDot | QDir::AllDirs,
                                       QDir::Name,
                                       completer ) );
   mpBaseDir->setCompleter( completer );

   mpMessage->setFrameShadow( QFrame::Raised );
   mpMessage->setFrameShape( QFrame::Box );
   mpPartymanInfo->setFrameShadow( QFrame::Raised );
   mpPartymanInfo->setFrameShape( QFrame::Box );
   mpUpdateButton->setCheckable( true );
   mpCleanupButton->setCheckable( true );
   mpImportButton->setCheckable( true );

   connect( browseButton, SIGNAL(clicked()),
            this, SLOT(setBaseDir()) );
   connect( mpUpdateButton, SIGNAL(clicked(bool)),
            this, SLOT(handleUpdate(bool)) );
   connect( mpCleanupButton, SIGNAL(clicked(bool)),
            this, SLOT(handleCleanup(bool)) );
   connect( mpImportButton, SIGNAL(clicked(bool)),
            this, SLOT(handleImport(bool)) );
   connect( mpBaseDir, SIGNAL(textChanged(QString)),
            this, SLOT(checkValidDir(QString)) );
   connect( mpDatabaseWorker, SIGNAL(progress(int,int)),
            this, SLOT(handleProgress(int,int)) );
   connect( mpDatabaseWorker, SIGNAL(finished()),
            this, SLOT(handleFinished()) );

   QVBoxLayout *layout = new QVBoxLayout;
   QHBoxLayout *rootLayout = new QHBoxLayout;
   rootLayout->addWidget( new QLabel( tr("Music Base:"), this ) );
   rootLayout->addWidget( mpBaseDir );
   rootLayout->addWidget( browseButton );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( mpUpdateButton );
   buttonLayout->addWidget( mpCleanupButton );
   buttonLayout->addWidget( mpImportButton );
   layout->addLayout( rootLayout );
   layout->addLayout( buttonLayout );
   layout->addWidget( mpMessage );
   layout->addWidget( mpPartymanInfo );
   layout->addStretch();
   setLayout(layout);
   mpBaseDir->setText( MySettings( "Global" ).VALUE_MUSICBASE );
   readPartymanConfig();
}


void DatabaseWidget::disableButtons( bool disable )
{
   mpUpdateButton->setDisabled( disable | !mPartymanLocal );
   mpCleanupButton->setDisabled( disable | !mPartymanLocal );
   mpImportButton->setDisabled( disable );
   if( !disable )
   {
      mpUpdateButton->setChecked( disable );
      mpCleanupButton->setChecked( disable );
      mpImportButton->setChecked( disable );
   }
}

void DatabaseWidget::handleUpdate( bool checked )
{
   if( !checked )
   {
      mpUpdateButton->setChecked( true );
      return;
   }
   disableButtons( true );
   mCheckedText   = tr( "files scanned" );
   mProcessedText = tr( "updated" );
   QString baseDir( MySettings( "Global" ).VALUE_MUSICBASE );
   if( !baseDir.isEmpty() )
   {
      mpDatabaseWorker->startUpdate( baseDir );
   }
   else
   {
      disableButtons( false );
   }
}


void DatabaseWidget::handleCleanup( bool checked )
{
   if( !checked )
   {
      mpCleanupButton->setChecked( true );
      return;
   }
   disableButtons( true );
   mCheckedText   = tr( "entries checked" );
   mProcessedText = tr( "cleaned" );
   mpDatabaseWorker->startCleanup();
}


void DatabaseWidget::handleImport( bool checked )
{
   if( !checked )
   {
      mpImportButton->setChecked( true );
      return;
   }
   disableButtons( true );
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::ExistingFile );
   fileDialog.setDirectory( mpBaseDir->text() );
   fileDialog.setFilter("Playlists (*.m3u)");
   fileDialog.setReadOnly( true );
   if( fileDialog.exec() )
   {
      mCheckedText   = tr( "files scanned" );
      mProcessedText = tr( "added" );
      mpDatabaseWorker->startImport( fileDialog.selectedFiles().at(0) );
   }
   else
   {
      disableButtons( false );
   }
}


void DatabaseWidget::setBaseDir()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpBaseDir->text() );
   fileDialog.setReadOnly( true );

   if( fileDialog.exec() )
   {
      MySettings settings( "Global" );
      QString result( fileDialog.selectedFiles().at(0) );
      mpBaseDir->setText( result );
      settings.setValue( "MusicBase", result.replace('\\','/') );
   }
}


void DatabaseWidget::checkValidDir( const QString &dirName )
{
   QFileInfo qfi( dirName );
   if( qfi.isDir() )
   {
      mpUpdateButton->setDisabled( false );
      MySettings settings( "Global" );
      settings.setValue( "MusicBase", QString( dirName ).replace('\\','/') );
   }
   else
   {
      mpUpdateButton->setDisabled( true );
   }
}


void DatabaseWidget::handleProgress( int checked, int processed )
{
   mpMessage->setText(
         QString("%1 %2, %3 %4.").arg( checked ).arg( mCheckedText )
                                 .arg( processed ).arg( mProcessedText ) );
}


void DatabaseWidget::handleFinished()
{
   mpMessage->setText( tr("Done: ") + mpMessage->text() );
   disableButtons( false );
   emit databaseUpdated();
}


void DatabaseWidget::readPartymanConfig( const QHostInfo &hi )
{
   MySettings partymanSettings( "Partyman" );

   mPartymanLocal = partymanSettings.value("DerMixDrun", true).toBool();
   if( !mPartymanLocal && (hi.lookupId() == -1) )
   {
      QHostInfo::lookupHost(partymanSettings.value("DerMixDhost", "localhost").toString(),
                            this, SLOT(readPartymanConfig(QHostInfo)));
      return;
   }
   mPartymanLocal |= (hi.error() != QHostInfo::NoError);
   mPartymanLocal |= hi.addresses().isEmpty();
   if( !mPartymanLocal )
   {
      mPartymanLocal |= (hi.addresses().first() == QHostAddress::LocalHost);
   }

   if( mPartymanLocal )
   {
      mpPartymanInfo->setText( tr("Partyman is set to local mode.\n"
                                  "Nonexistant files will be removed from database.\n"
                                  "Tags will be scanned." ) );
   }
   else
   {
      mpPartymanInfo->setText( tr("Partyman is set to remote mode.\n"
                                  "Nonexistant files will not be removed from database.\n"
                                  "Tags will not be scanned.") );
   }
   disableButtons( false );
}
