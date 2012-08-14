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
   setDockNestingEnabled( true );

   /* setting up playlist */
   mpPlaylistContent->setAlternatingRowColors( true );
   mpPlaylistContent->setAutoFillBackground( false );
   mpPlaylistContent->setHidden( false );
   mpPlaylistContent->addItems( Settings::value( Settings::PartymanPlaylist ) );
   mpPlaylistContent->addItems( Settings::value( Settings::PartymanPlaylistAppend ) );
   savePlaylist();
   setCentralWidget( mpControl );
   WidgetShot::addWidget( "Player", mpControl );

   /* setting up playlist widget */
   QDockWidget *dockPlaylist = setupDock( mpPlaylistContent, "Playlist", tr("Playlist"),
                                          QKeySequence(Qt::Key_F5) );

   /* settings up help text */
   QDockWidget *dockHelpText = setupDock( mpHelpText, "Help", tr("Help"),
                                          QKeySequence(Qt::Key_F1) );
   mpHelpText->setReadOnly( true );
   mpHelpText->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   mpHelpText->setOpenExternalLinks( true );
   mpHelpText->setSource( QUrl("qrc:/Usage.html") );

   /* settings up info panel */
   QDockWidget *dockTrackInfo = setupDock( mpTrackInfo, "Info", tr("Info"),
                                           QKeySequence(Qt::Key_F2) );

   /* setting up search */
   QDockWidget *dockSearch = setupDock( mpSearch, "Search", tr("Search"),
                                        QKeySequence(Qt::Key_F3) );

   /* setting up file system tree view */
   mpDockTreeView = setupDock( mpTreeView, "Browse", QString(),
                               QKeySequence(Qt::Key_F4) );
   mpTreeView->header()->hide();
   mpTreeView->setDragDropMode( QAbstractItemView::DragOnly );
   mpTreeView->setDragEnabled( true );
   mpTreeView->setSelectionMode( QAbstractItemView::ExtendedSelection );

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
   connect( mpSearch, SIGNAL(requestAdd(QStringList)),
            mpPlaylistContent, SLOT(addItems(QStringList)) );

   addDockWidget( Qt::BottomDockWidgetArea, dockPlaylist );
   addDockWidget( Qt::BottomDockWidgetArea, mpDockTreeView, Qt::Vertical );
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


QDockWidget *PartymanMainWindow::setupDock( QWidget *widget,
                                            const QString &name,
                                            const QString &label,
                                            const QKeySequence &shortCutKey )
{
   QDockWidget *dockWidget = new QDockWidget( label, this );
   dockWidget->setObjectName( name );
   dockWidget->setWidget( widget );
   QShortcut *shortCut = new QShortcut( shortCutKey, this );

   connect( shortCut, SIGNAL(activated()),
            dockWidget, SLOT(show()) );
   connect( shortCut, SIGNAL(activated()),
            dockWidget, SLOT(raise()) );
   connect( shortCut, SIGNAL(activated()),
            widget, SLOT(setFocus()) );
   connect( dockWidget, SIGNAL(visibilityChanged(bool)),
            this, SLOT(setDockFocus(bool)) );

   WidgetShot::addWidget( name, dockWidget );

   return dockWidget;
}


void PartymanMainWindow::setDockFocus( bool doit )
{
   if( doit )
   {
      QDockWidget *dw = qobject_cast<QDockWidget*>( sender() );
      if( dw )
      {
         QWidget *w = dw->widget();
         QMetaObject::invokeMethod( w, "setFocus", Qt::QueuedConnection );
      }
   }
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
      if( mpDatabase->getRandomTrack( &trackInfo, favoriteOnly, leastPlayed, *playedArtists, playFolder ) )
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
