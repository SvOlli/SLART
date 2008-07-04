/**
 * PlaylistWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QtGui>

#include "FileSysTreeModel.hpp"
#include "PlaylistContentWidget.hpp"
#include "PlaylistWidget.hpp"
#include "SearchWidget.hpp"
#include "TrackInfoWidget.hpp"
#include "ConfigDialog.hpp"
#include "Database.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


PlaylistWidget::PlaylistWidget( Database *database, ConfigDialog *config,
                                QWidget *parent, Qt::WindowFlags f )
: QWidget( parent, f )
, mpDatabase( database )
, mpConfig( config )
, mpTabs( new QTabWidget( this ) )
, mpPlaylistContent( new PlaylistContentWidget( true, this ) )
, mpTreeView( new FileSysTreeView( this ) )
, mpTreeModel( new FileSysTreeModel( this ) )
, mpNextTreeModel( 0 )
, mpTreeUpdate( new FileSysTreeUpdate() )
, mpSearch( new SearchWidget( database, this ) )
, mpTrackInfo( new TrackInfoWidget( database, this ) )
, mpHelpText( new QTextBrowser( this ) )
{
   MySettings settings;
   qsrand( time((time_t*)0) );
   
   mpTreeView->header()->hide();
   
   mpPlaylistContent->setSortingEnabled( false );
   mpPlaylistContent->setAlternatingRowColors( true );
   mpPlaylistContent->setAutoFillBackground( false );
   mpPlaylistContent->setHidden( false );
   mpPlaylistContent->addItems( settings.value("Playlist", QStringList()).toStringList()  );

   mpHelpText->setReadOnly( true );
   mpHelpText->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
   
   QHBoxLayout *layout = new QHBoxLayout;
   QSplitter *splitter = new QSplitter( Qt::Vertical, parent );
   mpTabs->setTabPosition( QTabWidget::South );
   mpTabs->addTab( mpTreeView, tr("Browser") );
   mpTabs->addTab( mpSearch, tr("Search") );
   mpTabs->addTab( mpTrackInfo, tr("Track Info") );
   mpTabs->addTab( mpHelpText, tr("Help") );
   mpTabs->setCurrentIndex( settings.value("CurrentTab", mpTabs->count()-1).toInt() );
   
   splitter->addWidget( mpPlaylistContent );
   splitter->addWidget( mpTabs );
   layout->addWidget( splitter );
#if QT_VERSION < 0x040300
   layout->setMargin( 0 );
#else
   layout->setContentsMargins( 0, 0, 0, 0 );
#endif
   
   connect( mpTreeView, SIGNAL(context(QModelIndex)),
            this, SLOT(addEntries(QModelIndex)) );
   connect( this, SIGNAL(expand(QModelIndex)),
            mpTreeView, SLOT(expand(QModelIndex)) );
   connect( mpPlaylistContent, SIGNAL(context(QModelIndex,int)),
            this, SLOT(deleteEntry(QModelIndex,int)) );
   connect( mpTabs, SIGNAL(currentChanged(int)),
            this, SLOT(handleTabChange(int)) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpTreeUpdate, SIGNAL(finished()),
            this, SLOT(finishBrowserUpdate()) );
   readConfig();
   
   mpHelpText->setOpenExternalLinks( true );
   mpHelpText->setSource( QUrl("qrc:/Usage.html") );
   setAcceptDrops( true );
   setLayout( layout );
}


PlaylistWidget::~PlaylistWidget()
{
   MySettings settings;
   QStringList playlist;
   int i;
   
   for( i = 0; i < mpPlaylistContent->count(); i++ )
   {
      playlist.append( mpPlaylistContent->item(i)->data(Qt::DisplayRole).toString() );
   }
   
   if( playlist.count() > 0 )
   {
      settings.setValue( "Playlist", playlist );
   }
   else
   {
      settings.remove( "Playlist" );
   }
}


void PlaylistWidget::deleteEntry( const QModelIndex &/*index*/, int key )
{
   switch( key )
   {
      case 0: /* right mouse button */
      case Qt::Key_Delete:
         mpPlaylistContent->removeSelectedItems();
         break;
      default:
         break;
   }
}


void PlaylistWidget::addEntries( QStringList *list, const QModelIndex &index )
{
   QModelIndex qmi;
   if( index.child(0,0).isValid() )
   {
      /* childs exists */
      for( int i = 0; (qmi=index.child( i, 0 )).isValid(); i++ )
      {
         addEntries( list, qmi );
      }
   }
   else
   {
      if( index.isValid() )
      {
         (*list) << mpTreeModel->getPath( index );
      }
   }
}


void PlaylistWidget::addEntries( const QModelIndex &index )
{
   QStringList entries;
   
   if( mpPlaylistContent->isHidden() )
   {
      return;
   }
   
   addEntries( &entries, index );
   
   mpPlaylistContent->addItems( entries );
   mpPlaylistContent->scrollToBottom();
}


void PlaylistWidget::addEntries( const QStringList &entries, bool atStart )
{
   if( atStart )
   {
      mpPlaylistContent->insertItems( 0, entries );
      mpPlaylistContent->scrollToTop();
   }
   else
   {
      mpPlaylistContent->addItems( entries );
      mpPlaylistContent->scrollToBottom();
   }
}


void PlaylistWidget::getNextTrack( QString *fileName )
{
   if( mpPlaylistContent->count() > 0 )
   {
      QListWidgetItem *qlwi = mpPlaylistContent->takeItem(0);
      mpPlaylistContent->setToolTip( QModelIndex() );
      *fileName = qlwi->text();
      delete qlwi;
   }
   else
   {
      TrackInfo trackInfo;
      if( mpDatabase->getTrack( &trackInfo, false, false ) )
      {
         *fileName = trackInfo.filePath();
      }
   }
}


void PlaylistWidget::dragEnterEvent( QDragEnterEvent *event )
{
   if( event->mimeData()->hasFormat("text/uri-list") )
   {
      event->acceptProposedAction();
   }
}


void PlaylistWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   
   if( mimeData->hasUrls() )
   {
      int i;
      
      QStringList dest;
      QFileInfo qfi;
      
      for( i = 0; i < mimeData->urls().size(); i++ )
      {
         qfi.setFile( mimeData->urls().at(i).toLocalFile() );
         
         if( qfi.isFile() )
         {
            if( qfi.suffix().startsWith( "m3u" ) )
            {
               QFile qf( qfi.canonicalFilePath() );
               qf.open( QIODevice::ReadOnly | QIODevice::Text );
               
               QByteArray line;
               QString filename;
               QString filebase( qfi.canonicalFilePath() + "/../" );
               while( !qf.atEnd() )
               {
                  line = qf.readLine();
                  filename = QString::fromLocal8Bit( line );
                  if( !filename.startsWith("#") )
                  {
                     if( filename.right(1) == QChar('\n') )
                     {
                        filename.chop(1);
                     }
                     if( !filename.startsWith( "/" ) )
                     {
                        qfi.setFile( filebase + filename );
                        filename = qfi.absoluteFilePath();
                     }
                     dest << filename;
                  }
               }
               qf.close();
            }
            else
            {
               dest << qfi.absoluteFilePath();
            }
         }
      }
      
      if( dest.size() > 0 )
      {
         addEntries( dest );
      }
   }
}


void PlaylistWidget::readConfig()
{
   QSettings settings( QApplication::organizationName(), "Global" );
   mpTreeView->setAnimated( settings.value( "AnimateViews", false ).toBool() );
}


void PlaylistWidget::handleTabChange( int tabNr )
{
   MySettings settings;
   
   switch( tabNr )
   {
      case 0:
         /* browser */
         mpTreeView->setFocus();
         break;
      case 1:
         /* search */
         mpSearch->setFocus();
         break;
      case 3:
         /* help */
         mpHelpText->setFocus();
         break;
      break;
      default:
         break;
   }
   
   settings.setValue( "CurrentTab", tabNr );
}


void PlaylistWidget::getTrack( const TrackInfo &trackInfo )
{
   mpTrackInfo->getTrack( trackInfo );
}


void PlaylistWidget::startBrowserUpdate()
{
   if( mpNextTreeModel )
   {
      return;
   }
   
   mpTreeView->setEnabled( false );
   mpNextTreeModel = new FileSysTreeModel( this );
   
   unsigned int retval = mpTreeUpdate->prepare( mpDatabase, mpNextTreeModel );
   if( retval > 0 )
   {
      mpTreeUpdate->start();
      mpTreeUpdate->setPriority( QThread::LowestPriority );
   }
   emit playlistIsValid( retval > 0 );
}


void PlaylistWidget::finishBrowserUpdate()
{
   int i;
   QModelIndex root, qmi;
   mpTreeView->setModel( mpNextTreeModel );
   delete mpTreeModel;
   mpTreeModel = mpNextTreeModel;
   
   mpTreeView->setRootIndex( root );
   for(i = 0; ; i++)
   {
      qmi = mpTreeModel->index( i, 0, root );
      if( !qmi.isValid() )
      {
         break;
      } 
      
      do
      {
         emit expand( qmi );
         qmi = mpTreeModel->index( 0, 0, qmi );
      }
      while( mpTreeModel->rowCount( qmi ) <= 1 );
      emit expand( qmi );
   }
   
   mpTreeView->setEnabled( true );
   mpNextTreeModel = 0;
}
