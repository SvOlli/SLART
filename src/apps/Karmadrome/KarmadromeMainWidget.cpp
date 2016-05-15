/*
 * src/apps/Karmadrome/KarmadromeMainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "KarmadromeMainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <GenericSatelliteHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>
#include <ScrollLine.hpp>
#include <TrackInfoWidget.hpp>
#include <WindowIconChanger.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "ButtonsWidget.hpp"
#include "ExportGroup.hpp"
#include "ExportGroupList.hpp"
#include "KarmadromeConfigDialog.hpp"
#include "ImportGroup.hpp"


KarmadromeMainWidget::KarmadromeMainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( DatabaseInterface::get() )
, mpSatellite( Satellite::get() )
, mpGenericSatelliteHandler( 0 )
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
, mpListButtons( new ButtonsWidget( tr("Groups:"), this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpAddButton( new QPushButton( tr("Add"), this ) )
, mpRemoveButton( new QPushButton( tr("Remove"), this ) )
, mpRemoveMenu( new QMenu( this ) )
, mpConfigDialog( new KarmadromeConfigDialog( this ) )
, mpTimer( new QTimer( this ) )
, mGroupNameToRemove()
, mGroups()
, mTrackInfo()
{
   QGridLayout *mainLayout   = new QGridLayout( this );

   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/Karmadrome/Icon.png" ) );

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

   mpDatabase->getGroups( this, SLOT(updateGroupNames(QStringList)) );
   mpDatabase->connectActivityIndicator( mpFileName, SLOT(setDisabled(bool)) );

   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(labelReadButton()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(updateLists()) );
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

   WidgetShot::addWidget( "Main", this );

   if( mpSatellite )
   {
      mpGenericSatelliteHandler = new GenericSatelliteHandler( mpSatellite, GenericSatelliteHandler::WithPingAndDialog, this );
      mpGenericSatelliteHandler->setConnectMsg( "P0R" );
      connect( mpGenericSatelliteHandler, SIGNAL(updateConfig()),
               mpConfigDialog, SLOT(readSettings()) );
      connect( mpSatellite, SIGNAL(received(QByteArray)),
               this, SLOT(handleSatellite(QByteArray)) );
      mpDatabase->registerUpdate( mpSatellite, "k0u" );
   }
   WindowIconChanger *wic = new WindowIconChanger( parent, QIcon(":/Common/DatabaseUp.png"), this );
   mpDatabase->connectActivityIndicator( wic, SLOT(changed(bool)) );
}


KarmadromeMainWidget::~KarmadromeMainWidget()
{
}


void KarmadromeMainWidget::addToList( QWidget *widget )
{
   QAbstractButton *button = qobject_cast<QAbstractButton*>(widget);
   if( !button )
   {
      return;
   }
   if( mTrackInfo.mID > 0 )
   {
      mTrackInfo.setGroup( button->text(), button->isChecked() );
      mpDatabase->updateTrackInfo( mTrackInfo );
      mpTrackInfo->getTrack( mTrackInfo );
      mpTimer->start();
   }
   else
   {
      button->setChecked( false );
   }
}


void KarmadromeMainWidget::sendK0u()
{
   mpSatellite->send( QByteArray("k0u") );
}


void KarmadromeMainWidget::handleSatellite( const QByteArray &msg )
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

            /* will remove itself using deleteLater() */
            new ExportGroup( message.at(1), message.at(2),
                              Settings::value( Settings::KarmadromeExportAsRelative ),
                              Settings::value( Settings::KarmadromeRandomizeExport ) );
         }
      }

      if( message.at(0) == "K0I" )
      {
         if( message.at(2).startsWith( "/" ) )
         {
            /* will remove itself using deleteLater() */
            new ImportGroup( message.at(1), message.at(2),
                              Settings::value( Settings::KarmadromeClearBeforeImport ) );
         }
      }
   }

   if( message.count() > 1 )
   {
      if( message.at(0) == "K0A" )
      {
         if( mGroups.contains( message.at(1) ) )
         {
            mTrackInfo.setGroup( message.at(1), !mTrackInfo.isInGroup( message.at(1) ) );
            mpDatabase->updateTrackInfo( mTrackInfo );
            mpTimer->start();
         }
      }

      if( message.at(0) == "p0p" )
      {
         mpFileName->setText( message.at(1) );
         mpFileName->setDragFileName( message.at(1) );
         mpDatabase->getTrackInfo( this, SLOT(updateTrackInfo(TrackInfo)), message.at(1) );
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
      if( mTrackInfo.isInDatabase() )
      {
         mpDatabase->getTrackInfo( this, SLOT(updateTrackInfo(TrackInfo)), mTrackInfo.mID );
      }
      else
      {
         mpDatabase->getTrackInfo( this, SLOT(updateTrackInfo(TrackInfo)), mpFileName->text() );
      }
   }
}


void KarmadromeMainWidget::updateLists()
{
   int i;

   mpListButtons->updateButtons( mGroups );
   mpExportMenu->clear();
   mpImportMenu->clear();
   mpRemoveMenu->clear();

   mpExportMenu->addAction( mpExportFavorite );
   mpImportMenu->addAction( mpImportFavorite );
   mpExportMenu->addAction( mpExportUnwanted );
   mpImportMenu->addAction( mpImportUnwanted );

   if( mGroups.count() > 0 )
   {
      mpExportMenu->addSeparator();
      mpImportMenu->addSeparator();
   }

   for( i = 0; i < mGroups.count(); i++ )
   {
      mpExportMenu->addAction( mGroups.at(i) );
      mpImportMenu->addAction( mGroups.at(i) );
      mpRemoveMenu->addAction( mGroups.at(i) );
   }
   mpListButtons->lockButtons( mTrackInfo.getGroups() );
}


void KarmadromeMainWidget::handleAdd()
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
         mpDatabase->insertGroup( folder );
         mpDatabase->getGroups( this, SLOT(updateGroupNames(QStringList)) );
      }
   }
}


void KarmadromeMainWidget::handleExport( QAction *action )
{
   setButtonsEnabled( false );
   QFileDialog dialog( this, tr("Rubberbandman: Export %1 To:").arg(action->text()) );
   dialog.setFileMode( QFileDialog::AnyFile );
   dialog.setNameFilter( tr("Playlist files (*.m3u)") );
   QString rqdir( Settings::value( Settings::KarmadromeExportDirectory ) );
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = Settings::value( Settings::KarmadromeImportDirectory );
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
      Settings::setValue( Settings::KarmadromeExportDirectory, dialog.directory().absolutePath() );
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
      ExportGroup *exportGroup = new ExportGroup( folder, fileName,
                                                     Settings::value( Settings::KarmadromeExportAsRelative ),
                                                     Settings::value( Settings::KarmadromeRandomizeExport ) );
      connect( exportGroup, SIGNAL(destroyed()),
               this, SLOT(setButtonsEnabled()) );
   }
   else
   {
      setButtonsEnabled();
   }
}


void KarmadromeMainWidget::handleImport( QAction *action )
{
   setButtonsEnabled( false );
   QFileDialog dialog( this, tr("Rubberbandman: Import %1 From:").arg(action->text()) );
   dialog.setFileMode( QFileDialog::ExistingFiles );
   dialog.setNameFilter( tr("Playlist files (*.m3u)") );
   QString rqdir( Settings::value( Settings::KarmadromeImportDirectory ) );
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = Settings::value( Settings::KarmadromeExportDirectory );
   }
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = QDir::currentPath();
   }
   dialog.setDirectory( rqdir );
   dialog.setAcceptMode( QFileDialog::AcceptOpen );
   if( dialog.exec() )
   {
      Settings::setValue( Settings::KarmadromeImportDirectory, dialog.directory().absolutePath() );
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
         ImportGroup *importGroup = new ImportGroup( folder, dialog.selectedFiles().at(i),
                                                        (!i) ? Settings::value( Settings::KarmadromeClearBeforeImport ) : false );
         connect( importGroup, SIGNAL(destroyed()),
                  this, SLOT(setButtonsEnabled()) );
      }
      QDir::setCurrent( cwd );
   }
   else
   {
      setButtonsEnabled();
   }
}


void KarmadromeMainWidget::handleRemove( QAction *action )
{
   setButtonsEnabled( false );
   if( QMessageBox::Ok == QMessageBox::question( this, QApplication::applicationName(),
                                                 tr("Are you sure you want to delete the folder\n") +
                                                 action->text(), QMessageBox::Ok | QMessageBox::Cancel ) )
   {
      mpDatabase->getGroup( this, SLOT(removeGroup(QStringList,QVariant)), action->text() );
   }
   else
   {
      setButtonsEnabled();
   }
}


void KarmadromeMainWidget::handleReadButton()
{
   mpFileName->setText( GlobalConfigWidget::getClipboard() );
   mpDatabase->getTrackInfo( this, SLOT(updateTrackInfo(TrackInfo)), mpFileName->text() );
}


void KarmadromeMainWidget::labelReadButton()
{
   int mode = Settings::value( Settings::GlobalClipboardMode );

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
}


void KarmadromeMainWidget::updateTrackInfo( const TrackInfo &trackInfo )
{
   mTrackInfo = trackInfo;
   mpListButtons->setDisabled( !mTrackInfo.isInDatabase() );
   mpTrackInfo->getTrack( mTrackInfo );
   mpListButtons->lockButtons( mTrackInfo.getGroups() );
   mpFileName->setDragInfo( trackInfo );
}


void KarmadromeMainWidget::removeGroup( const QStringList &entries, const QVariant &payload )
{
   QString folder( payload.toString() );
   if( folder.isEmpty() )
   {
      return;
   }
   foreach( const QString &entry, entries )
   {
      mpDatabase->getTrackInfo( this, SLOT(removeGroupFromTrack(TrackInfo,QVariant)), entry, payload );
   }
   mpDatabase->deleteGroup( folder );
   mpDatabase->getGroups( this, SLOT(updateGroupNames(QStringList)) );
   setButtonsEnabled();
}


void KarmadromeMainWidget::removeGroupFromTrack( const TrackInfo &ti, const QVariant &payload )
{
   QString folder( payload.toString() );
   if( folder.isEmpty() )
   {
      return;
   }
   TrackInfo trackInfo( ti );
   trackInfo.setGroup( folder, false );
   mpDatabase->updateTrackInfo( trackInfo );
}


void KarmadromeMainWidget::setButtonsEnabled( bool enabled )
{
   mpRemoveButton->setEnabled( enabled );
   mpExportButton->setEnabled( enabled );
   mpImportButton->setEnabled( enabled );
}


void KarmadromeMainWidget::updateGroupNames( const QStringList &list )
{
   mGroups = list;
   mGroups.sort();
   updateLists();
}
