/**
 * src/apps/Karmadrome/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Database.hpp>
#include <GenericSatMsgHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <ScrollLine.hpp>
#include <TrackInfoWidget.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "ButtonsWidget.hpp"
#include "ConfigDialog.hpp"
#include "ImportExport.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpSatellite( Satellite::get( this ) )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite ) )
, mpImportExport( new ImportExport( mpDatabase) )
, mpFileName( new ScrollLine( this ) )
, mpTrackInfo( new TrackInfoWidget( mpDatabase, QByteArray("k0u"), false, this ) )
, mpReadButton( new QPushButton( this ) )
, mpExportButton( new QPushButton( tr("Export m3u"), this ) )
, mpExportMenu( new QMenu( this ) )
, mpExportFavorite( new QAction( tr("Favorite"), this ) )
, mpExportUnwanted( new QAction( tr("No Auto"), this ) )
, mpImportButton( new QPushButton( tr("Import m3u"), this ) )
, mpImportMenu( new QMenu( this ) )
, mpImportFavorite( new QAction( tr("Favorite"), this ) )
, mpImportUnwanted( new QAction( tr("No Auto"), this ) )
, mpListButtons( new ButtonsWidget( tr("Folders:"), this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpAddButton( new QPushButton( tr("Add"), this ) )
, mpRemoveButton( new QPushButton( tr("Remove"), this ) )
, mpRemoveMenu( new QMenu( this ) )
, mpConfigDialog( new ConfigDialog( this ) )
, mpTimer( new QTimer( this ) )
, mPlaylists()
, mTrackInfo()
{
   qsrand( time((time_t*)0) );
   QGridLayout *mainLayout   = new QGridLayout( this );
   
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );
   
   mpTimer->setSingleShot( true );
   mpTimer->setInterval( 2000 );
   
   mpExportButton->setMenu( mpExportMenu );
   mpImportButton->setMenu( mpImportMenu );
   mpRemoveButton->setMenu( mpRemoveMenu );

   mainLayout->addWidget( mpFileName,       0, 0, 1, 3 );
   mainLayout->addWidget( mpTrackInfo,      1, 0, 1, 3 );
   mainLayout->addWidget( mpReadButton,     2, 0 );
   mainLayout->addWidget( mpExportButton,   2, 1 );
   mainLayout->addWidget( mpImportButton,   2, 2 );
   mainLayout->addWidget( mpListButtons,    3, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton, 4, 0 );
   mainLayout->addWidget( mpAddButton,      4, 1 );
   mainLayout->addWidget( mpRemoveButton,   4, 2 );
   
   mainLayout->setRowStretch( 0, 0 );
   mainLayout->setRowStretch( 1, 0 );
   mainLayout->setRowStretch( 2, 0 );
   mainLayout->setRowStretch( 3, 1 );
   mainLayout->setRowStretch( 4, 0 );
   
   setLayout( mainLayout );
   
   updateLists();

   connect( mpSatellite, SIGNAL(received(const QByteArray &)),
            this, SLOT(handleSatellite(const QByteArray &)) );
   
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(labelReadButton()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(updateLists()) );
   connect( mpGenericSatMsgHandler, SIGNAL(updateConfig()),
            mpConfigDialog, SLOT(readSettings()) );
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(sendK0u()) );
   
   connect( mpReadButton, SIGNAL(clicked()),
            this, SLOT(handleReadButton()) );
   connect( mpExportMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(handleExport(QAction *)) );
   connect( mpImportMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(handleImport(QAction *)) );
   connect( mpListButtons, SIGNAL(clicked(QWidget*)),
            this, SLOT(addToList(QWidget*)) );
   connect( mpAddButton, SIGNAL(clicked()),
            this, SLOT(handleAdd()) );
   connect( mpRemoveMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(handleRemove(QAction *)) );
   connect( mpTrackInfo, SIGNAL(checkboxClicked(const TrackInfo&)),
            this, SLOT(updateTrackInfo(const TrackInfo &)));

   labelReadButton();
   
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   
   mpListButtons->setDisabled( true );

   WidgetShot::addWidget( "MainWidget", this );

   mpSatellite->send( "P0R" );
}


MainWidget::~MainWidget()
{
   delete mpDatabase;
}


void MainWidget::addToList( QWidget *widget )
{
   QAbstractButton *button = qobject_cast<QAbstractButton*>(widget);
   if( !button )
   {
      return;
   }
   if( mTrackInfo.mID > 0 )
   {
      mTrackInfo.setFolder( button->text(), button->isChecked() );
      mpDatabase->updateTrackInfo( &mTrackInfo );
      mpTrackInfo->getTrack( mTrackInfo );
      mpTimer->start();
   }
   else
   {
      button->setChecked( false );
   }
}


void MainWidget::sendK0u()
{
   mpSatellite->send( QByteArray("k0u") );
}


void MainWidget::handleSatellite( const QByteArray &msg )
{
   mpGenericSatMsgHandler->handle( msg );

   QStringList message( Satellite::split( msg ) );
   if( message.count() > 2 )
   {
      if( message.at(0) == "K0E" )
      {
         if( message.at(2).startsWith( "/" ) )
         {
            QFileInfo qfi( message.at(2) );
            if( qfi.isDir() )
            {
               return;
            }
            if( qfi.isFile() )
            {
               QMessageBox::StandardButton button;
               button = QMessageBox::question( this, QString( QApplication::applicationName() + tr(": Overwrite %1")).arg(message.at(2)),
                                               QString(tr("Overwrite %1 ?")).arg(message.at(2)),
                                               QMessageBox::Ok | QMessageBox::Cancel );
               if( button != QMessageBox::Ok )
               {
                  return;
               }
            }
            
            MySettings settings;
            mpImportExport->exportM3u( message.at(1), message.at(2),
                                       settings.VALUE_EXPORTASRELATIVE,
                                       settings.VALUE_RANDOMIZEEXPORT );
         }
      }
      
      if( message.at(0) == "K0I" )
      {
         if( message.at(2).startsWith( "/" ) )
         {
            mpImportExport->importM3u( message.at(1), message.at(2),
                                       MySettings().VALUE_CLEARBEFOREIMPORT );
         }
      }
   }
   
   if( message.count() > 1 )
   {
      if( message.at(0) == "K0A" )
      {
         if( mpDatabase->getFolders().contains( message.at(1) ) )
         {
            mTrackInfo.setFolder( message.at(1), !mTrackInfo.isInFolder( message.at(1) ) );
            mpDatabase->updateTrackInfo( &mTrackInfo );
            mpTimer->start();
         }
      }
      
      if( message.at(0) == "p0p" )
      {
         mpFileName->setText( message.at(1) );
         mpFileName->setDragFileName( mpFileName->text() );
         mpListButtons->setDisabled( !mpDatabase->getTrackInfo( &mTrackInfo, message.at(1) ) );
         mpTrackInfo->getTrack( mTrackInfo );
         mpListButtons->lockButtons( mTrackInfo.getFolders() );
      }
   }
   
   if( message.at(0) == "p0s" )
   {
      mTrackInfo.clear();
      mpFileName->clear();
      mpFileName->setDragFileName();
      mpListButtons->setDisabled( true );
      mpTrackInfo->getTrack( mTrackInfo );
   }
   
   if( (message.at(0) == "p0u") || 
       (message.at(0) == "r0u") )
   {
      mpDatabase->getTrackInfo( &mTrackInfo );
      mpTrackInfo->getTrack( mTrackInfo );
      mpListButtons->lockButtons( mTrackInfo.getFolders() );
   }
}


void MainWidget::updateLists()
{
   int i;
   mPlaylists = mpDatabase->getFolders();
   mPlaylists.sort();
   
   mpListButtons->updateButtons( mPlaylists );
   mpExportMenu->clear();
   mpImportMenu->clear();
   mpRemoveMenu->clear();
   
   mpExportMenu->addAction( mpExportFavorite );
   mpImportMenu->addAction( mpImportFavorite );
   mpExportMenu->addAction( mpExportUnwanted );
   mpImportMenu->addAction( mpImportUnwanted );
   
   if( mPlaylists.count() > 0 )
   {
      mpExportMenu->addSeparator();
      mpImportMenu->addSeparator();
   }
   
   for( i = 0; i < mPlaylists.count(); i++ )
   {
      mpExportMenu->addAction( mPlaylists.at(i) );
      mpImportMenu->addAction( mPlaylists.at(i) );
      mpRemoveMenu->addAction( mPlaylists.at(i) );
   }
   mpListButtons->lockButtons( mTrackInfo.getFolders() );
}


void MainWidget::handleAdd()
{
   bool ok;
   QString folder( QInputDialog::getText( this, QApplication::applicationName(),
                                          tr("Enter name of folder:"),
                                          QLineEdit::Normal, QString(), &ok ) );
   if( ok && !folder.isEmpty() )
   {
      if( folder.contains( "|" ) )
      {
         QMessageBox::warning( this, QApplication::applicationName(),
                               tr("Name is not valid.") );
      }
      else
      {
         mpDatabase->insertFolder( folder );
         updateLists();
      }
   }
}


void MainWidget::handleExport( QAction *action )
{
   MySettings settings;
   QFileDialog dialog( this, QString(tr("Rubberbandman: Export %1 To:")).arg(action->text()) );
   dialog.setFileMode( QFileDialog::AnyFile );
   dialog.setFilter( tr("Playlist files (*.m3u)") );
   QString rqdir( settings.VALUE_EXPORTDIRECTORY );
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = settings.VALUE_IMPORTDIRECTORY;
   }
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = QDir::currentPath();
   }
   dialog.setDirectory( rqdir );
   dialog.setAcceptMode( QFileDialog::AcceptSave );
   dialog.setConfirmOverwrite( false ); // handled in exportM3u()
   if( dialog.exec() )
   {
      settings.setValue( "ExportDirectory", dialog.directory().absolutePath() );
      if( dialog.selectedFiles().count() == 0 )
      {
         return;
      }
      QString fileName( dialog.selectedFiles().at(0) );
      if( !fileName.endsWith( ".m3u", Qt::CaseInsensitive ) )
      {
         fileName.append( ".m3u" );
      }
      QString folder( action->text() );
      if( action == mpExportFavorite )
      {
         folder = QChar(1);
      }
      else if( action == mpExportUnwanted )
      {
         folder = QChar(2);
      }
      mpImportExport->exportM3u( folder, fileName, 
                                 settings.VALUE_EXPORTASRELATIVE,
                                 settings.VALUE_RANDOMIZEEXPORT );
   }
}


void MainWidget::handleImport( QAction *action )
{
   MySettings settings;
   QFileDialog dialog( this, QString(tr("Rubberbandman: Import %1 From:")).arg(action->text()) );
   dialog.setFileMode( QFileDialog::ExistingFiles );
   dialog.setFilter( tr("Playlist files (*.m3u)") );
   QString rqdir( settings.VALUE_IMPORTDIRECTORY );
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = settings.VALUE_EXPORTDIRECTORY;
   }
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = QDir::currentPath();
   }
   dialog.setDirectory( rqdir );
   dialog.setAcceptMode( QFileDialog::AcceptOpen );
   if( dialog.exec() )
   {
      TrackInfo trackInfo;
      settings.setValue( "ImportDirectory", dialog.directory().absolutePath() );
      QString cwd( QDir::currentPath() );
      
      QString folder( action->text() );
      if( action == mpImportFavorite )
      {
         folder = QChar(1);
      }
      else if( action == mpImportUnwanted )
      {
         folder = QChar(2);
      }
      
      for( int i = 0; i < dialog.selectedFiles().count(); i++ )
      {
         mpImportExport->importM3u( folder, dialog.selectedFiles().at(i),
                                    (!i) ? settings.VALUE_CLEARBEFOREIMPORT : false );
      }
      QDir::setCurrent( cwd );
   }
}


void MainWidget::handleRemove( QAction *action )
{
   if( QMessageBox::Ok == QMessageBox::question( this, QApplication::applicationName(),
                                                 tr("Are you sure you want to delete the folder\n") +
                                                 action->text(), QMessageBox::Ok | QMessageBox::Cancel ) )
   {
      TrackInfo trackInfo;
      QStringList entries( mpDatabase->getFolder( action->text() ) );
      mpDatabase->beginTransaction();
      for( int i = 0; i < entries.count(); i++ )
      {
         if( mpDatabase->getTrackInfo( &trackInfo, entries.at(i) ) )
         {
            trackInfo.setFolder( action->text(), false );
            mpDatabase->updateTrackInfo( &trackInfo );
         }
      }
      mpDatabase->endTransaction( true );
      mpDatabase->deleteFolder( action->text() );
      updateLists();
   }
}


void MainWidget::handleReadButton()
{
   mpFileName->setText( GlobalConfigWidget::getClipboard() );
   mpFileName->setDragFileName( mpFileName->text() );
   mpListButtons->setDisabled( !mpDatabase->getTrackInfo( &mTrackInfo, GlobalConfigWidget::getClipboard() ) );
   mpTrackInfo->getTrack( mTrackInfo );
   mpListButtons->lockButtons( mTrackInfo.getFolders() );
}


void MainWidget::labelReadButton()
{
   MySettings settings( "Global" );
   int mode = settings.VALUE_CLIPBOARDMODE;

   switch( mode )
   {
      case 1:
      case 3:
         mpReadButton->setText( tr("Read Selection") );
         mpReadButton->setHidden( false );
         break;
      case 2:
      case 4:
         mpReadButton->setText( tr("Read Clipboard") );
         mpReadButton->setHidden( false );
         break;
      default:
         mpReadButton->setText( QString() );
         mpReadButton->setHidden( true );
         break;
   }
   mpSatellite->restart();
}


void MainWidget::updateTrackInfo( const TrackInfo &trackInfo )
{
   mTrackInfo = trackInfo;
}
