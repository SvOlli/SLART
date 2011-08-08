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
#include <DatabaseInterface.hpp>
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
#include "ExportFolder.hpp"
#include "ExportFolderList.hpp"
#include "ImportFolder.hpp"


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( DatabaseInterface::get() )
, mpSatellite( Satellite::get() )
, mpGenericSatMsgHandler( new GenericSatMsgHandler( mpSatellite, GenericSatMsgHandler::WithPingAndDialog ) )
, mpFileName( new ScrollLine( this ) )
, mpTrackInfo( new TrackInfoWidget( false, this ) )
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
, mpCurrentAction( 0 )
, mFolders()
, mTrackInfo()
{
   mpDatabase->registerUpdate( mpSatellite, "k0u" );
   QGridLayout *mainLayout   = new QGridLayout( this );

   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );

   mpTimer->setSingleShot( true );
   mpTimer->setInterval( 500 );

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

   mpDatabase->getFolders( this, "updateFolderNames" );

   mpDatabase->connectActivityIndicator( mpFileName, SLOT(setDisabled(bool)) );
   connect( mpSatellite, SIGNAL(received(QByteArray)),
            this, SLOT(handleSatellite(QByteArray)) );

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
   connect( mpExportMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(handleExport(QAction*)) );
   connect( mpImportMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(handleImport(QAction*)) );
   connect( mpListButtons, SIGNAL(clicked(QWidget*)),
            this, SLOT(addToList(QWidget*)) );
   connect( mpAddButton, SIGNAL(clicked()),
            this, SLOT(handleAdd()) );
   connect( mpRemoveMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(handleRemove(QAction*)) );
   connect( mpTrackInfo, SIGNAL(checkboxClicked(TrackInfo)),
            this, SLOT(updateTrackInfo(TrackInfo)));

   labelReadButton();

   mpSettingsButton->setObjectName( QString("SettingsButton") );

   mpListButtons->setDisabled( true );

   WidgetShot::addWidget( "MainWidget", this );

   mpSatellite->send( "P0R" );
}


MainWidget::~MainWidget()
{
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
      mpDatabase->updateTrackInfo( mTrackInfo );
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
               button = QMessageBox::question( this, QString( QApplication::applicationName() + ": " +
                                                              tr("Overwrite %1")).arg(message.at(2)),
                                               tr("Overwrite %1 ?").arg(message.at(2)),
                                               QMessageBox::Ok | QMessageBox::Cancel );
               if( button != QMessageBox::Ok )
               {
                  return;
               }
            }

            MySettings settings;
            ExportFolder *exportFolder = new ExportFolder( message.at(1), message.at(2),
                                                           settings.VALUE_EXPORTASRELATIVE,
                                                           settings.VALUE_RANDOMIZEEXPORT );
            Q_UNUSED( exportFolder );
         }
      }

      if( message.at(0) == "K0I" )
      {
         if( message.at(2).startsWith( "/" ) )
         {
            ImportFolder *importFolder = new ImportFolder( message.at(1), message.at(2),
                                                           MySettings().VALUE_CLEARBEFOREIMPORT );
            Q_UNUSED( importFolder );
         }
      }
   }

   if( message.count() > 1 )
   {
      if( message.at(0) == "K0A" )
      {
         if( mFolders.contains( message.at(1) ) )
         {
            mTrackInfo.setFolder( message.at(1), !mTrackInfo.isInFolder( message.at(1) ) );
            mpDatabase->updateTrackInfo( mTrackInfo );
            mpTimer->start();
         }
      }

      if( message.at(0) == "p0p" )
      {
         mpFileName->setText( message.at(1) );
         mpFileName->setDragFileName( message.at(1) );
         mpDatabase->getTrackInfo( this, "updateTrackInfo", message.at(1) );
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
#if 1
      if( mTrackInfo.isInDatabase() )
      {
         mpDatabase->getTrackInfo( this, "updateTrackInfo", mTrackInfo.mID );
      }
      else
      {
         mpDatabase->getTrackInfo( this, "updateTrackInfo", mpFileName->text() );
      }
#else
      mpDatabase->getTrackInfo( this, "updateTrackInfo", mpFileName->text() );
#endif
   }
}


void MainWidget::updateLists()
{
   int i;

   mpListButtons->updateButtons( mFolders );
   mpExportMenu->clear();
   mpImportMenu->clear();
   mpRemoveMenu->clear();

   mpExportMenu->addAction( mpExportFavorite );
   mpImportMenu->addAction( mpImportFavorite );
   mpExportMenu->addAction( mpExportUnwanted );
   mpImportMenu->addAction( mpImportUnwanted );

   if( mFolders.count() > 0 )
   {
      mpExportMenu->addSeparator();
      mpImportMenu->addSeparator();
   }

   for( i = 0; i < mFolders.count(); i++ )
   {
      mpExportMenu->addAction( mFolders.at(i) );
      mpImportMenu->addAction( mFolders.at(i) );
      mpRemoveMenu->addAction( mFolders.at(i) );
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
   setButtonsEnabled( false );
   QFileDialog dialog( this, tr("Rubberbandman: Export %1 To:").arg(action->text()) );
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
      ExportFolder *exportFolder = new ExportFolder( folder, fileName,
                                                     settings.VALUE_EXPORTASRELATIVE,
                                                     settings.VALUE_RANDOMIZEEXPORT );
      connect( exportFolder, SIGNAL(destroyed()),
               this, SLOT(setButtonsEnabled()) );
   }
   else
   {
      setButtonsEnabled();
   }
}


void MainWidget::handleImport( QAction *action )
{
   MySettings settings;
   setButtonsEnabled( false );
   QFileDialog dialog( this, tr("Rubberbandman: Import %1 From:").arg(action->text()) );
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
         ImportFolder *importFolder = new ImportFolder( folder, dialog.selectedFiles().at(i),
                                                        (!i) ? settings.VALUE_CLEARBEFOREIMPORT : false );
         connect( importFolder, SIGNAL(destroyed()),
                  this, SLOT(setButtonsEnabled()) );
      }
      QDir::setCurrent( cwd );
   }
   else
   {
      setButtonsEnabled();
   }
}


void MainWidget::handleRemove( QAction *action )
{
   setButtonsEnabled( false );
   if( QMessageBox::Ok == QMessageBox::question( this, QApplication::applicationName(),
                                                 tr("Are you sure you want to delete the folder\n") +
                                                 action->text(), QMessageBox::Ok | QMessageBox::Cancel ) )
   {
      mpCurrentAction = action;

      mpDatabase->getFolder( this, "removeFolder", action->text() );
   }
   else
   {
      setButtonsEnabled();
   }
}


void MainWidget::handleReadButton()
{
   mpFileName->setText( GlobalConfigWidget::getClipboard() );
   mpFileName->setDragFileName( mpFileName->text() );
   mpDatabase->getTrackInfo( this, "updateTrackInfo", mpFileName->text() );
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
   mpListButtons->setDisabled( !mTrackInfo.isInDatabase() );
   mpTrackInfo->getTrack( mTrackInfo );
   mpListButtons->lockButtons( mTrackInfo.getFolders() );
}


void MainWidget::removeFolder( const QStringList &entries )
{
   foreach( const QString &entry, entries )
   {
      mpDatabase->getTrackInfo( this, "removeFolderFromTrack", entry );
   }
   mpDatabase->deleteFolder( mpCurrentAction->text() );
   mpCurrentAction = 0;
   updateLists();
   setButtonsEnabled();
}


void MainWidget::removeFolderFromTrack( const TrackInfo &ti )
{
   TrackInfo trackInfo( ti );
   trackInfo.setFolder( mpCurrentAction->text(), false );
   mpDatabase->updateTrackInfo( trackInfo );
}


void MainWidget::setButtonsEnabled( bool enabled )
{
   mpRemoveButton->setEnabled( enabled );
   mpExportButton->setEnabled( enabled );
   mpImportButton->setEnabled( enabled );
}


void MainWidget::updateFolderNames( const QStringList &list )
{
   mFolders = list;
   mFolders.sort();
   updateLists();
}
