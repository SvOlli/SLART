/*
 * src/apps/Partyman/PartymanMainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PartymanMainWindow.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <Database.hpp>
#include <MainWindowCheckClose.hpp>
#include <PasswordChecker.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "PartymanConfigDialog.hpp"
#include "ControlWidget.hpp"
#include "FileSysTreeModel.hpp"
#include "FileSysTreeUpdate.hpp"
#include "FileSysTreeView.hpp"
#include "PlaylistContentWidget.hpp"
#include "SearchTrackWidget.hpp"
#include "TrackInfoWidget.hpp"


PartymanMainWindow::PartymanMainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mProhibitCloseWindow( false )
, mForbidMove( 100 )
, mpDatabase( new Database() )
, mpConfig( new PartymanConfigDialog( mpDatabase, this ) )
, mpPlaylistContent( new PlaylistContentWidget( mpDatabase, true, this ) )
, mpControl( new ControlWidget( mpDatabase, mpConfig, this ) )
, mpHelpText( new QTextBrowser( this ) )
, mpTrackInfo( new TrackInfoWidget( mpDatabase, true, this ) )
, mpSearch( new SearchTrackWidget( mpDatabase, this ) )
, mpTreeView( new FileSysTreeView( this ) )
, mpTreeModel( new FileSysTreeModel( this ) )
, mpNextTreeModel( 0 )
, mpTreeUpdate( new FileSysTreeUpdate() )
, mpDockTreeView( 0 )
, mCurrentFile()
, mNextFile()
{
   qsrand( time((time_t*)0) );
   setUnifiedTitleAndToolBarOnMac(true);
   WidgetShot::addWidget( "Main", this );
   mpDatabase->registerUpdate( Satellite::get(), "p0u" );

   setAttribute( Qt::WA_AlwaysShowToolTips, true );
   setWindowIcon( QIcon( ":/PartymanSmile.png" ) );

   /* setting up playlist */
   mpPlaylistContent->setAlternatingRowColors( true );
   mpPlaylistContent->setAutoFillBackground( false );
   mpPlaylistContent->setHidden( false );
   mpPlaylistContent->addItems( Settings::value( Settings::PartymanPlaylist ) );
   mpPlaylistContent->addItems( Settings::value( Settings::PartymanPlaylistAppend ) );
   savePlaylist();
   setCentralWidget( mpPlaylistContent );
   WidgetShot::addWidget( "Playlist", mpPlaylistContent );

   /* setting up control widget */
   QDockWidget *dockControl = new QDockWidget( tr("Player"), this );
   dockControl->setObjectName( "Player" );
   dockControl->setWidget( mpControl );
   dockControl->setFeatures( QDockWidget::DockWidgetMovable |
                             QDockWidget::DockWidgetFloatable );
   WidgetShot::addWidget( "Player", dockControl );

   /* settings up help text */
   QDockWidget *dockHelpText = new QDockWidget( tr("Help"), this );
   dockHelpText->setObjectName( "Help" );
   dockHelpText->setWidget( mpHelpText );
   mpHelpText->setReadOnly( true );
   mpHelpText->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   mpHelpText->setOpenExternalLinks( true );
   mpHelpText->setSource( QUrl("qrc:/Usage.html") );
   WidgetShot::addWidget( "Help", dockHelpText );

   /* settings up info panel */
   QDockWidget *dockTrackInfo = new QDockWidget( tr("Info"), this );
   dockTrackInfo->setObjectName( "Info" );
   dockTrackInfo->setWidget( mpTrackInfo );
   WidgetShot::addWidget( "Info", dockTrackInfo );

   /* setting up search */
   QDockWidget *dockSearch = new QDockWidget( tr("Search"), this );
   dockSearch->setObjectName( "Search" );
   dockSearch->setWidget( mpSearch );
   WidgetShot::addWidget( "Search", dockSearch );

   /* setting up file system tree view */
   mpDockTreeView = new QDockWidget( this );
   mpDockTreeView->setObjectName( "Browse" );
   mpDockTreeView->setWidget( mpTreeView );
   mpTreeView->header()->hide();
   mpTreeView->setDragDropMode( QAbstractItemView::DragOnly );
   mpTreeView->setDragEnabled( true );
   mpTreeView->setSelectionMode( QAbstractItemView::ExtendedSelection );
   WidgetShot::addWidget( "Browse", mpDockTreeView );

   connect( mpTreeView, SIGNAL(context(QModelIndex)),
            this, SLOT(addEntries(QModelIndex)) );
   connect( this, SIGNAL(expand(QModelIndex)),
            mpTreeView, SLOT(expand(QModelIndex)) );
   connect( mpPlaylistContent, SIGNAL(context(QModelIndex,int)),
            this, SLOT(deleteEntry(QModelIndex,int)) );
   connect( mpPlaylistContent, SIGNAL(contentChanged()),
            this, SLOT(savePlaylist()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpTreeUpdate, SIGNAL(finished()),
            this, SLOT(finishBrowserUpdate()) );
   connect( mpControl, SIGNAL(requestAddToPlaylist(QStringList,bool)),
            this, SLOT(addEntries(QStringList,bool)) );
   connect( mpControl, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
   connect( this, SIGNAL(playlistIsValid(bool)),
            mpControl, SLOT(allowConnect(bool)) );
   connect( mpControl, SIGNAL(trackUpdate()),
            this, SLOT(updateTrackInfo()) );
   connect( mpConfig, SIGNAL(updateBrowser()),
            this, SLOT(startBrowserUpdate()) );
   connect( PasswordChecker::get(), SIGNAL(enabled(bool)),
            mpControl, SLOT(handleKioskMode(bool)) );
   connect( PasswordChecker::get(), SIGNAL(enabled(bool)),
            this, SLOT(prohibitClose(bool)) );
   connect( mpControl, SIGNAL(requestTab(QString)),
            this, SLOT(showTab(QString)) );

   QShortcut *f1 = new QShortcut( QKeySequence(Qt::Key_F1), this );
   QShortcut *f2 = new QShortcut( QKeySequence(Qt::Key_F2), this );
   QShortcut *f3 = new QShortcut( QKeySequence(Qt::Key_F3), this );
   QShortcut *f4 = new QShortcut( QKeySequence(Qt::Key_F4), this );
   QShortcut *f5 = new QShortcut( QKeySequence(Qt::Key_F5), this );

   connect( f1, SIGNAL(activated()),
            dockHelpText, SLOT(show()) );
   connect( f1, SIGNAL(activated()),
            dockHelpText, SLOT(raise()) );
   connect( f1, SIGNAL(activated()),
            mpHelpText, SLOT(setFocus()) );

   connect( f2, SIGNAL(activated()),
            dockTrackInfo, SLOT(show()) );
   connect( f2, SIGNAL(activated()),
            dockTrackInfo, SLOT(raise()) );
   connect( f2, SIGNAL(activated()),
            mpTrackInfo, SLOT(setFocus()) );

   connect( f3, SIGNAL(activated()),
            dockSearch, SLOT(show()) );
   connect( f3, SIGNAL(activated()),
            dockSearch, SLOT(raise()) );
   connect( f3, SIGNAL(activated()),
            mpSearch, SLOT(setFocus()) );

   connect( f4, SIGNAL(activated()),
            mpDockTreeView, SLOT(show()) );
   connect( f4, SIGNAL(activated()),
            mpDockTreeView, SLOT(raise()) );
   connect( f4, SIGNAL(activated()),
            mpTreeView, SLOT(setFocus()) );

   connect( f5, SIGNAL(activated()),
            dockControl, SLOT(show()) );
   connect( f5, SIGNAL(activated()),
            dockControl, SLOT(raise()) );
   connect( f5, SIGNAL(activated()),
            mpControl, SLOT(setFocus()) );

   addDockWidget( Qt::TopDockWidgetArea, dockControl );
   addDockWidget( Qt::BottomDockWidgetArea, mpDockTreeView );
   addDockWidget( Qt::BottomDockWidgetArea, dockSearch );
   addDockWidget( Qt::BottomDockWidgetArea, dockTrackInfo );
   addDockWidget( Qt::BottomDockWidgetArea, dockHelpText );

   tabifyDockWidget( mpDockTreeView, dockSearch );
   tabifyDockWidget( mpDockTreeView, dockTrackInfo );
   tabifyDockWidget( mpDockTreeView, dockHelpText );

   restoreGeometry( Settings::value( Settings::CommonGeometry ) );
   restoreState( Settings::value( Settings::CommonState ) );
   QList<QDockWidget*> docks( findChildren<QDockWidget*>() );
   foreach( QDockWidget *dock, docks )
   {
      restoreDockWidget( dock );
   }

   readConfig();

   QMetaObject::invokeMethod( this, "startBrowserUpdate",
                              Qt::QueuedConnection,
                              Q_ARG( bool, true ) );
}


PartymanMainWindow::~PartymanMainWindow()
{
   if( mpTreeUpdate->isRunning() )
   {
      mpTreeUpdate->cancel();
      mpTreeUpdate->quit();
      mpTreeUpdate->wait();
   }
}


/* Evil workaround: in the first fifty events revert all move requests */
bool PartymanMainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( event->type() == QEvent::Move )
      {
         restoreGeometry( Settings::value( Settings::CommonGeometry ) );
      }
      mForbidMove--;
   }
   return QMainWindow::event( event );
}


void PartymanMainWindow::showTab( const QString &name )
{
   QList<QDockWidget*> list = findChildren<QDockWidget*>( name );
   if( list.size() == 1 )
   {
      list.at(0)->show();
      list.at(0)->raise();
   }
}


void PartymanMainWindow::changeTitle( const QIcon &icon, const QString &title )
{
   if( !icon.isNull() )
   {
      setWindowIcon( icon );
   }
   if( !title.isNull() )
   {
      setWindowTitle( title );
   }
}


void PartymanMainWindow::prohibitClose( bool prohibit )
{
   mProhibitCloseWindow = prohibit;
}


void PartymanMainWindow::closeEvent( QCloseEvent *event )
{
   if( mProhibitCloseWindow )
   {
      if( !(PasswordChecker::get()->unlock()) )
      {
         event->ignore();
         return;
      }
   }
   Settings::setValue( Settings::CommonGeometry, saveGeometry() );
   Settings::setValue( Settings::CommonState, saveState() );
   event->accept();
}


void PartymanMainWindow::deleteEntry( const QModelIndex &/*index*/, int key )
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


void PartymanMainWindow::addEntries( QStringList *list, const QModelIndex &index )
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


void PartymanMainWindow::addEntries( const QModelIndex &index )
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


void PartymanMainWindow::addEntries( const QStringList &entries, bool atStart )
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


void PartymanMainWindow::getNextTrack( QString *fileName )
{
   QStringList playlistAppend( Settings::value( Settings::PartymanPlaylistAppend ) );
   if( playlistAppend.size() > 0 )
   {
      mpPlaylistContent->addItems( playlistAppend );
   }

   if( mpPlaylistContent->count() > 0 )
   {
      *fileName = mpPlaylistContent->takeFilePath(0);
      savePlaylist();
   }
   else
   {
      int randomTries = Settings::value( Settings::PartymanRandomTries );
      QStringList playedArtists( Settings::value( Settings::PartymanPlayedArtists ) );
      bool favoriteOnly = Settings::value( Settings::PartymanPlayOnlyFavorite );
      bool leastPlayed  = Settings::value( Settings::PartymanPlayOnlyLeastPlayed );
      int notAgainCount = Settings::value( Settings::PartymanPlayNotAgainCount );
      QString playFolder( Settings::value( Settings::PartymanPlayFolder ) );

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
      Settings::setValue( Settings::PartymanPlayedArtists, playedArtists );
   }
}


void PartymanMainWindow::setTrackInfoFavoriteUnwanted( bool favorite, bool unwanted )
{
   mpTrackInfo->setFavoriteUnwanted( favorite, unwanted );
}


bool PartymanMainWindow::getRandomTrack( QString *fileName, QStringList *playedArtists, int randomTries,
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


void PartymanMainWindow::readConfig()
{
   mpTreeView->setAnimated( Settings::value( Settings::GlobalAnimateViews ) );
   mpControl->readConfig();
}


void PartymanMainWindow::setTrackInfo( const TrackInfo &trackInfo )
{
   mpTrackInfo->getTrack( trackInfo );
}


void PartymanMainWindow::startBrowserUpdate( bool initial )
{
   if( mpNextTreeModel )
   {
      return;
   }

   mpDockTreeView->setWindowTitle( tr("*Wait*") );
   mpTreeView->setEnabled( false );
   mpNextTreeModel = new FileSysTreeModel( this );

   int retval = mpTreeUpdate->prepare( mpDatabase, mpNextTreeModel );
   if( retval > 0 )
   {
      mpTreeUpdate->start();
      mpTreeUpdate->setPriority( QThread::LowestPriority );
   }

   if( initial )
   {
      if( retval <= 0 )
      {
         QMessageBox::warning( this, QApplication::applicationName() + ": " + tr("database empty"),
                               tr("The database is empty. Please run Rubberbandman.") );
      }
      else
      {
         emit playlistIsValid( true );
         if( Settings::value( Settings::PartymanAutoConnect ) && (retval > 0) )
         {
            mpControl->initConnect();
         }
         else
         {
            mpControl->initDisconnect();
         }
      }
   }
}


void PartymanMainWindow::finishBrowserUpdate()
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
   mpDockTreeView->setWindowTitle( tr("Browse") );
}


void PartymanMainWindow::updateTrackInfo()
{
   mpTrackInfo->update();
}


void PartymanMainWindow::setCurrentNext( const QString &current, const QString &next )
{
   mCurrentFile = current;
   mNextFile    = next;
}


void PartymanMainWindow::savePlaylist()
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

   Settings::setValue( Settings::PartymanPlaylist, playlist );
   Settings::setValue( Settings::PartymanPlaylistAppend, QStringList() );
}
