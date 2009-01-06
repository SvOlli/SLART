/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "ButtonsWidget.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#include "GlobalConfigWidget.hpp"
#include "ScrollLine.hpp"
#include "TrackInfoWidget.hpp"
#include "Database.hpp"

#include <QtGui>

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpFileName( new ScrollLine( this ) )
, mpTrackInfo( new TrackInfoWidget( mpDatabase, QString("k0u"), this ) )
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
, mSLARTCom()
, mPlaylists()
, mTrackInfo()
{
   qsrand( time((time_t*)0) );
   QGridLayout *mainLayout   = new QGridLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   mpTimer->setSingleShot( true );
   mpTimer->setInterval( 2000 );
   
   QLabel *pLogo = new QLabel( this );
   pLogo->setText( QApplication::applicationName() );
   pLogo->setAlignment( Qt::AlignCenter );
   pLogo->setFrameShadow( QFrame::Raised );
   pLogo->setFrameShape( QFrame::Box );
   
   mpExportButton->setMenu( mpExportMenu );
   mpImportButton->setMenu( mpImportMenu );
   mpRemoveButton->setMenu( mpRemoveMenu );

   mainLayout->addWidget( pLogo,            0, 0, 1, 3 );
   mainLayout->addWidget( mpFileName,       1, 0, 1, 3 );
   mainLayout->addWidget( mpTrackInfo,      2, 0, 1, 3 );
   mainLayout->addWidget( mpReadButton,     3, 0 );
   mainLayout->addWidget( mpExportButton,   3, 1 );
   mainLayout->addWidget( mpImportButton,   3, 2 );
   mainLayout->addWidget( mpListButtons,    4, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton, 5, 0 );
   mainLayout->addWidget( mpAddButton,      5, 1 );
   mainLayout->addWidget( mpRemoveButton,   5, 2 );
   
   setLayout( mainLayout );
   
   updateLists();

   connect( &mSLARTCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLART(QStringList)) );
   
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

   labelReadButton();
   mSLARTCom.resetReceiver();
   
   mpListButtons->setDisabled( true );
   MySettings().sendUdpMessage( "P0R", "Partyman" );
}


void MainWidget::addToList( QWidget *widget )
{
   QPushButton *pb = (QPushButton*)widget;
   if( mTrackInfo.mID > 0 )
   {
      mTrackInfo.setFolder( pb->text(), pb->isChecked() );
      mpDatabase->updateTrackInfo( &mTrackInfo );
      mpTimer->start();
   }
   else
   {
      pb->setChecked( false );
   }
}


void MainWidget::sendK0u()
{
   MySettings().sendNotification( QString("k0u") );
}


void MainWidget::handleSLART( const QStringList &message )
{
   if( message.count() > 2 )
   {
      if( message.at(0) == "K0E" )
      {
         if( message.at(2).startsWith( "/" ) )
         {
            if( message.at(1).startsWith( "|F", Qt::CaseInsensitive ) )
            {
               exportM3u( QChar(1), message.at(2) );
            }
            else if( message.at(1).startsWith( "|U", Qt::CaseInsensitive ) )
            {
               exportM3u( QChar(2), message.at(2) );
            }
            else
            {
               exportM3u( message.at(1), message.at(2) );
            }
         }
      }
      
      if( message.at(0) == "K0I" )
      {
         if( message.at(2).startsWith( "/" ) )
         {
            if( message.at(1).startsWith( "|F", Qt::CaseInsensitive ) )
            {
               importM3u( QChar(1), message.at(2) );
            }
            else if( message.at(1).startsWith( "|U", Qt::CaseInsensitive ) )
            {
               importM3u( QChar(2), message.at(2) );
            }
            else
            {
               importM3u( message.at(1), message.at(2) );
            }
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
         mpListButtons->setDisabled( !mpDatabase->getTrackInfo( &mTrackInfo, message.at(1) ) );
         mpTrackInfo->getTrack( mTrackInfo );
         mpListButtons->lockButtons( mTrackInfo.getFolders() );
      }
   }
   
   if( message.at(0) == "p0s" )
   {
      mTrackInfo.clear();
      mpFileName->clear();
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
   QString rqdir( settings.value( "ExportDirectory", QString() ).toString() );
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = settings.value( "ImportDirectory", QString() ).toString();
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
      exportM3u( folder, fileName );
   }
}


void MainWidget::handleImport( QAction *action )
{
   MySettings settings;
   QFileDialog dialog( this, QString(tr("Rubberbandman: Import %1 From:")).arg(action->text()) );
   dialog.setFileMode( QFileDialog::ExistingFiles );
   dialog.setFilter( tr("Playlist files (*.m3u)") );
   QString rqdir( settings.value( "ImportDirectory", QString() ).toString() );
   if( !QFileInfo( rqdir ).isDir() )
   {
      rqdir = settings.value( "ExportDirectory", QString() ).toString();
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
      if( settings.value( "ClearBeforeImport", false ).toBool() )
      {
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
      }
      for( int i = 0; i < dialog.selectedFiles().count(); i++ )
      {
         importM3u( action->text(), dialog.selectedFiles().at(i) );
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
   mpListButtons->setDisabled( !mpDatabase->getTrackInfo( &mTrackInfo, GlobalConfigWidget::getClipboard() ) );
   mpTrackInfo->getTrack( mTrackInfo );
}


void MainWidget::labelReadButton()
{
   MySettings settings( "Global" );
   int mode = settings.value( "ClipboardMode", 0 ).toInt();

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


void MainWidget::exportM3u( const QString &folder, const QString &fileName )
{
   MySettings settings;
   QFile m3uFile( fileName );
   if( m3uFile.exists() )
   {
      QMessageBox::StandardButton button;
      button = QMessageBox::question( this, QString( QApplication::applicationName() + tr(": Overwrite %1")).arg(fileName),
                                      QString(tr("Overwrite %1 ?")).arg(fileName),
                                      QMessageBox::Ok | QMessageBox::Cancel );
      if( button != QMessageBox::Ok )
      {
         return;
      }
   }
   if( !m3uFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
   {
      return;
   }
   QDir dir( QFileInfo( m3uFile.fileName() ).absolutePath() );
   QStringList entries( mpDatabase->getFolder( folder ) );
   if( settings.value( "RandomizeExport", false ).toBool() )
   {
      QStringList randomized;
      while( entries.count() )
      {
         randomized.append( entries.takeAt( qrand() % entries.count() ) );
      }
      entries = randomized;
   }
   if( settings.value( "ExportAsRelative", false ).toBool() )
   {
      for( int i = 0; i < entries.count(); i++ )
      {
         m3uFile.write( dir.relativeFilePath( entries.at(i) ).toLocal8Bit() );
         m3uFile.write( "\n" );
      }
   }
   else
   {
      for( int i = 0; i < entries.count(); i++ )
      {
         m3uFile.write( entries.at(i).toLocal8Bit() );
         m3uFile.write( "\n" );
      }
   }
   m3uFile.close();
}


void MainWidget::importM3u( const QString &folder, const QString &fileName )
{
   TrackInfo trackInfo;
   QFile m3uFile( fileName );
   if( m3uFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
   {
      QString fileName;
      QString fileBase( m3uFile.fileName() + "/../" );
      QFileInfo qfi;
      mpDatabase->beginTransaction();
      while( !m3uFile.atEnd() )
      {
         fileName = QString::fromLocal8Bit( m3uFile.readLine() );
         if( !fileName.startsWith("#") )
         {
            if( fileName.right(1) == QChar('\n') )
            {
               fileName.chop(1);
            }
            if( !fileName.startsWith( "/" ) )
            {
               /* a bit of an ugly trick, but gets the job done better than most
                  other solutions */
               qfi.setFile( fileBase + fileName );
               fileName = qfi.absoluteFilePath();
            }
            if( mpDatabase->getTrackInfo( &trackInfo, fileName ) )
            {
               if( folder == QChar(1) )
               {
                  trackInfo.setFlag( TrackInfo::Favorite, true );
               }
               else if( folder == QChar(2) )
               {
                  trackInfo.setFlag( TrackInfo::Unwanted, true );
               }
               else
               {
                  trackInfo.setFolder( folder, true );
               }
               mpDatabase->updateTrackInfo( &trackInfo );
            }
         }
      }
      mpDatabase->endTransaction( true );
      m3uFile.close();
   }
}
