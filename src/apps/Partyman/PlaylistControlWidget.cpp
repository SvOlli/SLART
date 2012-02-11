/*
 * src/apps/Partyman/PlaylistControlWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlaylistControlWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <TrackInfoWidget.hpp>
#include <GlobalConfigWidget.hpp>
#include <Database.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "FileSysTreeModel.hpp"
#include "PlaylistContentWidget.hpp"
#include "SearchWidget.hpp"


PlaylistControlWidget::PlaylistControlWidget( Database *database, ConfigDialog *config,
                                QWidget *parent, Qt::WindowFlags f )
: QWidget( parent, f )
, mpDatabase( database )
, mpConfig( config )
, mpTabs( new QTabWidget( this ) )
, mpPlaylistContent( new PlaylistContentWidget( database, true, this ) )
, mpTreeView( new FileSysTreeView( this ) )
, mpTreeModel( new FileSysTreeModel( this ) )
, mpNextTreeModel( 0 )
, mpTreeUpdate( new FileSysTreeUpdate() )
, mpSearch( new SearchWidget( database, this ) )
, mpTrackInfo( new TrackInfoWidget( database, true, this ) )
, mpHelpText( new QTextBrowser( this ) )
, mpSplitter( new QSplitter( Qt::Vertical, parent ) )
, mpFKeyMapper( new QSignalMapper( this ) )
, mCurrentFile()
, mNextFile()
{
   MySettings settings;
   qsrand( time((time_t*)0) );

   mpDatabase->registerUpdate( Satellite::get(), "p0u" );

   mpTreeView->header()->hide();
   mpTreeView->setDragDropMode( QAbstractItemView::DragOnly );
   mpTreeView->setDragEnabled( true );
   mpTreeView->setSelectionMode( QAbstractItemView::ExtendedSelection );

   mpPlaylistContent->setAlternatingRowColors( true );
   mpPlaylistContent->setAutoFillBackground( false );
   mpPlaylistContent->setHidden( false );
   mpPlaylistContent->addItems( settings.value( "Playlist", QStringList() ).toStringList() );
   QStringList playlistAppend( settings.value( "PlaylistAppend", QStringList() ).toStringList() );
   if( playlistAppend.size() > 0 )
   {
      settings.remove( "PlaylistAppend" );
      mpPlaylistContent->addItems( playlistAppend );
   }
   savePlaylist();

   mpHelpText->setReadOnly( true );
   mpHelpText->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   mpHelpText->setOpenExternalLinks( true );
   mpHelpText->setSource( QUrl("qrc:/Usage.html") );

   QHBoxLayout *layout = new QHBoxLayout;
   mpTabs->setTabPosition( QTabWidget::South );
   mpTabs->addTab( mpHelpText, tr("Help") );
   mpTabs->addTab( mpTrackInfo, tr("Info") );
   mpTabs->addTab( mpSearch, tr("Search") );
   mpTabs->addTab( mpTreeView, tr("Browse") );
   mpTabs->setCurrentIndex( settings.value("CurrentTab", 0).toInt() );

   mpSplitter->addWidget( mpPlaylistContent );
   mpSplitter->addWidget( mpTabs );
   QPoint regSizes( settings.value( "SplitterSizes", QPoint(1,1) ).toPoint() );
   QList<int> sizes;
   sizes << regSizes.x() << regSizes.y();
   mpSplitter->setSizes( sizes );
   layout->addWidget( mpSplitter );
   layout->setContentsMargins( 0, 0, 0, 0 );

   connect( mpTreeView, SIGNAL(context(QModelIndex)),
            this, SLOT(addEntries(QModelIndex)) );
   connect( this, SIGNAL(expand(QModelIndex)),
            mpTreeView, SLOT(expand(QModelIndex)) );
   connect( mpPlaylistContent, SIGNAL(context(QModelIndex,int)),
            this, SLOT(deleteEntry(QModelIndex,int)) );
   connect( mpPlaylistContent, SIGNAL(contentChanged()),
            this, SLOT(savePlaylist()) );
   connect( mpTabs, SIGNAL(currentChanged(int)),
            this, SLOT(handleTabChange(int)) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpTreeUpdate, SIGNAL(finished()),
            this, SLOT(finishBrowserUpdate()) );

   QShortcut *f1 = new QShortcut( QKeySequence(Qt::Key_F1), this );
   QShortcut *f2 = new QShortcut( QKeySequence(Qt::Key_F2), this );
   QShortcut *f3 = new QShortcut( QKeySequence(Qt::Key_F3), this );
   QShortcut *f4 = new QShortcut( QKeySequence(Qt::Key_F4), this );

   connect( f1, SIGNAL(activated()),
            mpFKeyMapper, SLOT(map()) );
   connect( f2, SIGNAL(activated()),
            mpFKeyMapper, SLOT(map()) );
   connect( f3, SIGNAL(activated()),
            mpFKeyMapper, SLOT(map()) );
   connect( f4, SIGNAL(activated()),
            mpFKeyMapper, SLOT(map()) );
   mpFKeyMapper->setMapping( f1, 0 );
   mpFKeyMapper->setMapping( f2, 1 );
   mpFKeyMapper->setMapping( f3, 2 );
   mpFKeyMapper->setMapping( f4, 3 );

   connect( mpFKeyMapper, SIGNAL(mapped(int)),
            mpTabs, SLOT(setCurrentIndex(int)) );

   readConfig();

   setLayout( layout );
}


PlaylistControlWidget::~PlaylistControlWidget()
{
   MySettings settings;

   if( mpTreeUpdate->isRunning() )
   {
      mpTreeUpdate->cancel();
      mpTreeUpdate->quit();
      mpTreeUpdate->wait();
   }

   QList<int> sizes = mpSplitter->sizes();
   if( sizes.count() > 1 )
   {
      settings.setValue( "SplitterSizes", QPoint(sizes.at(0),sizes.at(1)) );
   }
}


void PlaylistControlWidget::deleteEntry( const QModelIndex &/*index*/, int key )
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


void PlaylistControlWidget::addEntries( QStringList *list, const QModelIndex &index )
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


void PlaylistControlWidget::addEntries( const QModelIndex &index )
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


void PlaylistControlWidget::addEntries( const QStringList &entries, bool atStart )
{
   int i;
   QStringList checkedEntries;

   for( i = 0; i < entries.count(); i++ )
   {
      if( mpDatabase->getTrackInfo( 0, entries.at(i) ) )
      {
         checkedEntries.append( entries.at(i) );
      }
      else if( QFileInfo( entries.at(i) ).isFile() )
      {
         checkedEntries.append( entries.at(i) );
      }
   }
   mpPlaylistContent->addItems( checkedEntries, atStart );
   if( atStart )
   {
      mpPlaylistContent->scrollToTop();
   }
   else
   {
      mpPlaylistContent->scrollToBottom();
   }
}


void PlaylistControlWidget::getNextTrack( QString *fileName )
{
   MySettings settings;

   QStringList playlistAppend( settings.value( "PlaylistAppend", QStringList() ).toStringList() );
   if( playlistAppend.size() > 0 )
   {
      settings.remove( "PlaylistAppend" );
      mpPlaylistContent->addItems( playlistAppend );
   }

   if( mpPlaylistContent->count() > 0 )
   {
      *fileName = mpPlaylistContent->takeFilePath(0);
      savePlaylist();
   }
   else
   {
      int randomTries = settings.VALUE_RANDOMTRIES;
      QStringList playedArtists( settings.value("PlayedArtists").toStringList() );
      bool favoriteOnly = settings.VALUE_PLAYONLYFAVORITE;
      bool leastPlayed  = settings.VALUE_PLAYONLYLEASTPLAYED;
      int notAgainCount = settings.VALUE_PLAYNOTAGAINCOUNT;
      QString playFolder( settings.VALUE_PLAYFOLDER );

      if( !getRandomTrack( fileName, &playedArtists, randomTries, favoriteOnly, leastPlayed, playFolder ) )
      {
         if( !getRandomTrack( fileName, &playedArtists, randomTries, favoriteOnly, false, playFolder ) )
         {
            if( !getRandomTrack( fileName, &playedArtists, randomTries, false, false, playFolder ) )
            {
               getRandomTrack( fileName, &playedArtists, randomTries, false, false, QString() );
            }
         }
      }
      while( playedArtists.count() > notAgainCount )
      {
         playedArtists.takeLast();
      }
      settings.setValue( "PlayedArtists", playedArtists );
   }
}


void PlaylistControlWidget::setTrackInfoFavoriteUnwanted( bool favorite, bool unwanted )
{
   mpTrackInfo->setFavoriteUnwanted( favorite, unwanted );
}


bool PlaylistControlWidget::getRandomTrack( QString *fileName, QStringList *playedArtists, int randomTries,
                                     bool favoriteOnly, bool leastPlayed, const QString &playFolder )
{
   TrackInfo trackInfo;

   for( int i = 0; i < randomTries; i++ )
   {
      if( mpDatabase->getRandomTrack( &trackInfo, favoriteOnly, leastPlayed, playFolder ) )
      {
         *fileName = trackInfo.filePath();

         if( !playedArtists->contains( trackInfo.mArtist.simplified(), Qt::CaseInsensitive ) )
         {
            playedArtists->prepend( trackInfo.mArtist.simplified() );

            return true;
         }
      }
   }
   return false;
}


void PlaylistControlWidget::readConfig()
{
   mpTreeView->setAnimated( MySettings("Global").VALUE_ANIMATEVIEWS );
   mpSplitter->setOrientation( MySettings().VALUE_SPLITTERVERTICAL ? Qt::Horizontal : Qt::Vertical );
}


void PlaylistControlWidget::handleTabChange( int tabNr )
{
   if( (tabNr < 0) || (tabNr >= mpTabs->count()) )
   {
      return;
   }
   if( mpTabs->currentIndex() != tabNr )
   {
      mpTabs->setCurrentIndex( tabNr );
      return;
   }

   MySettings().setValue( "CurrentTab", tabNr );

   switch( tabNr )
   {
      case 0:
         /* help */
         mpHelpText->setFocus();
         break;
      case 2:
         /* search */
         mpSearch->setFocus();
         break;
      case 3:
         /* browser */
         mpTreeView->setFocus();
         break;
      default:
         break;
   }
}


void PlaylistControlWidget::getTrack( const TrackInfo &trackInfo )
{
   mpTrackInfo->getTrack( trackInfo );
}


void PlaylistControlWidget::startBrowserUpdate()
{
   if( mpNextTreeModel )
   {
      return;
   }

   mpTreeView->setEnabled( false );
   mpNextTreeModel = new FileSysTreeModel( this );

   int retval = mpTreeUpdate->prepare( mpDatabase, mpNextTreeModel );
   if( retval > 0 )
   {
      mpTreeUpdate->start();
      mpTreeUpdate->setPriority( QThread::LowestPriority );
      mpTabs->setTabText( 3, tr("*Wait*") );
   }
   emit playlistIsValid( retval > 0 );
}


void PlaylistControlWidget::finishBrowserUpdate()
{
   int i;
   QModelIndex root, qmi;
   mpTreeView->setModel( mpNextTreeModel );
   delete mpTreeModel;
   mpTreeModel = mpNextTreeModel;

   mpTreeView->setRootIndex( root );
   mpTabs->setTabText( 3, tr("Browse") );
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


void PlaylistControlWidget::updateTrackInfo()
{
   mpTrackInfo->update();
}


void PlaylistControlWidget::setCurrentNext( const QString &current, const QString &next )
{
   mCurrentFile = current;
   mNextFile    = next;
}


void PlaylistControlWidget::savePlaylist()
{
   QStringList playlist;

   if( !mCurrentFile.isEmpty() )
   {
      playlist << mCurrentFile;
   }
   if( !mNextFile.isEmpty() )
   {
      playlist << mNextFile;
   }

   playlist << mpPlaylistContent->allFilePaths();

   if( playlist.count() > 0 )
   {
      MySettings().setValue( "Playlist", playlist );
   }
   else
   {
      MySettings().remove( "Playlist" );
   }
}
