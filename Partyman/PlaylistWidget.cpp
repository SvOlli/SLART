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
#include "ConfigDialog.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


PlaylistWidget::PlaylistWidget( ConfigDialog *config, QWidget *parent, Qt::WindowFlags f )
: QWidget( parent, f )
, mpTabs( new QTabWidget( this ) )
, mpPlaylistContent( new PlaylistContentWidget( true, this ) )
, mpTreeView( new FileSysTreeView( this ) )
, mpTreeModel( new FileSysTreeModel( this ) )
, mpSearch( new SearchWidget( this ) )
, mpHelpText( new QTextBrowser( this ) )
, mpConfig( config )
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
   QBitArray bitset( mM3uData.count() );
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
   
   for( i = 0; i < mRandomList.count(); i++ )
   {
      bitset.setBit( mRandomList.at(i) );
   }
   settings.setValue( "RandomList", bitset );
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
      (*list) << mpTreeModel->getPath( index );
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
   int i;
   
   if( mpPlaylistContent->count() > 0 )
   {
      QListWidgetItem *qlwi = mpPlaylistContent->takeItem(0);
      mpPlaylistContent->setToolTip( QModelIndex() );
      *fileName = qlwi->text();
      delete qlwi;
   }
   else
   {
      if( !mRandomList.size() )
      {
         for( i = 0; i < mM3uData.size(); i++ )
         {
            mRandomList.append(i);
         }
      }
      i = qrand() % mRandomList.size();
      
      *fileName = mM3uData.at( mRandomList.takeAt(i) );
   }
}


QStringList PlaylistWidget::search( const QRegExp &rx ) const
{
   return mM3uData.filter( rx );
}


void PlaylistWidget::dragEnterEvent( QDragEnterEvent *event )
{
   if( event->mimeData()->hasFormat("text/plain") )
   {
      event->acceptProposedAction();
   }
}


void PlaylistWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   int i;
   
   if( mimeData->hasText() )
   {
      QStringList src( mimeData->text().remove('\r').split("\n",QString::SkipEmptyParts) );
      QStringList dest;
      
      for( i = 0; i < src.size(); i++ )
      {
         QUrl qu( src.at(i) );
         QFileInfo qfi( qu.toLocalFile() );
         if( qfi.isFile() )
         {
            dest << qfi.absoluteFilePath();
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
      case 2:
         /* help */
         mpHelpText->setFocus();
         break;
      break;
      default:
         break;
   }
   
   settings.setValue( "CurrentTab", tabNr );
}


void PlaylistWidget::readM3u()
{
   MySettings settings;
   int i, size;
   QString listfilename( settings.value( "DatabaseFilename", QString() ).toString() );
   if( (mM3uFileName == listfilename) && (mM3uData.size() > 0) )
   {
      return;
   }
   mM3uFileName = listfilename;

   QFile m3uFile( listfilename );
   if( !m3uFile.exists() )
   {
      emit playlistIsValid( false );
      return;
   }
   
   QProgressDialog progress( tr("<center><img src=':/PartymanSmile.gif'>&nbsp;"
                             "&nbsp;<img src=':/PartymanWriting.gif'><br>"
                             "Loading Playlist...</center>"),
                             QString(), 0, m3uFile.size(), this, Qt::Dialog | Qt::Popup );
   progress.setWindowTitle( QApplication::applicationName() );
   progress.setWindowIcon( QIcon(":/PartymanSmile.gif") );
   progress.setWindowOpacity ( 0.85 );
   progress.setModal( true );
   progress.setValue( 0 );
   QCoreApplication::processEvents();
   
   mM3uData.clear();
   mpTreeModel->clear();
   m3uFile.open( QIODevice::ReadOnly | QIODevice::Text );
   
   i = 0;
   size = 0;
   while (!m3uFile.atEnd())
   {
      QByteArray line = m3uFile.readLine();
      size += line.size();
      /* a bit of magic for not calling update too ofter */
      if( 100*size/m3uFile.size() > 0 )
      {
         progress.setValue( size );
         i++;
         QCoreApplication::processEvents();
      }
      
      QString filename(QString::fromLocal8Bit(line));
      if( filename.left(1) != "#" )
      {
         if( filename.right(1) == QChar('\n') )
         {
            filename.chop(1);
         }
         if( !filename.startsWith( "/" ) )
         {
            /* a bit of an ugly trick, but gets the job done better than most
               other solutions */
            filename = QFileInfo( listfilename + "/../" + filename ).absoluteFilePath();
         }
         mM3uData << filename;
         mpTreeModel->addModelData( filename );
      }
   }
   m3uFile.close();
   
   QModelIndex root, qmi;
   mpTreeView->setModel( mpTreeModel );
   mpTreeView->setRootIndex( root );
   
   if( mM3uData.count() > 0 )
   {
      mM3uData.sort();
   }
   else
   {
      emit playlistIsValid( false );
      return;
   }
   
   mRandomList.clear();
   /* reconstruct randomlist, if possible */
   QBitArray bitset( settings.value("RandomList", QBitArray()).toBitArray() );
   if( bitset.count() == mM3uData.count() )
   {
      for( int i = 0; i < bitset.count(); i++ )
      {
         if( bitset.at(i) )
         {
            mRandomList << i;
         }
      }
   }
   
   QCoreApplication::processEvents();
   /* expand the obivous */
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

   mpSearch->search();
   emit playlistIsValid( true );
}
