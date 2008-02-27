/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "ButtonsWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFileName( new QLineEdit( this ) )
, mpListButtons( new ButtonsWidget( tr("Karma Lists"), this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpAddButton( new QPushButton( tr("Add"), this ) )
, mpRemoveButton( new QPushButton( tr("Remove"), this ) )
, mpRemoveMenu( new QMenu( this ) )
, mSLATCom()
, mPlaylists( MySettings().value( "Playlists", QStringList() ).toStringList() )
{
   QGridLayout *mainLayout   = new QGridLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );
   
   mpFileName->setReadOnly( true );
   mpRemoveButton->setMenu( mpRemoveMenu );

   mainLayout->addWidget( mpLogo,           0, 0, 1, 3 );
   mainLayout->addWidget( mpFileName,       1, 0, 1, 3 );
   mainLayout->addWidget( mpListButtons,    2, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton, 3, 0, 1, 1 );
   mainLayout->addWidget( mpAddButton,      3, 1, 1, 1 );
   mainLayout->addWidget( mpRemoveButton,   3, 2, 1, 1 );
   
   setLayout( mainLayout );
   
   updateLists();

   connect( mpSettingsButton, SIGNAL(clicked()),
            this, SLOT(handleSettings()) );
   connect( &mSLATCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLAT(QStringList)) );
#if 0
   connect( &mSLATCom, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );
#endif
   
   connect( mpListButtons, SIGNAL(clicked(const QString &)),
            this, SLOT(addToList(const QString &)) );
   connect( mpAddButton, SIGNAL(clicked()),
            this, SLOT(handleAdd()) );
   connect( mpRemoveMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(handleRemove(QAction *)) );
   
   mSLATCom.resetReceiver();
}


void MainWidget::addToList( const QString &listfilename )
{
   int i;
   QFile m3uFile( listfilename );
   QStringList list( mpFileName->text() );
   
   if( list.at(0).isEmpty() )
   {
      return;
   }
   
   m3uFile.open( QIODevice::ReadOnly | QIODevice::Text );
   while (!m3uFile.atEnd())
   {
      QByteArray line( m3uFile.readLine() );
      
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
         list << filename;
      }
   }
   m3uFile.close();
   
   list.sort();
   
   m3uFile.open( QIODevice::WriteOnly | QIODevice::Text );
   
   for( i = 0; i < list.count(); i++ )
   {
      QByteArray line( list.at(i).toLocal8Bit() );
      line.append('\n');
      
      m3uFile.write( line );
   }
   
   m3uFile.close();
}


void MainWidget::handleSettings()
{
#if 0
   mpConfig->readSettings();
   mpConfig->exec();
#endif
}


void MainWidget::handleSLAT( const QStringList &message )
{
   if( message.count() > 1 )
   {
      if( message.at(0) == "p0p" )
      {
         mpFileName->setText( message.at(1) );
         mpFileName->setCursorPosition( 0 );
         mpFileName->setCursorPosition( message.at(1).size() );
      }
   }
}


void MainWidget::updateLists()
{
   int i;
   mPlaylists.sort();
   
   mpListButtons->updateButtons( mPlaylists );
   mpRemoveMenu->clear();
   for( i = 0; i < mPlaylists.count(); i++ )
   {
      int lastSlash = mPlaylists.at(i).lastIndexOf( '/' );
      int lastDot   = mPlaylists.at(i).lastIndexOf( '.' );
      QString label( mPlaylists.at(i).mid( lastSlash+1, lastDot-lastSlash-1 ) );
      
      QAction *action = mpRemoveMenu->addAction( label );
      action->setToolTip( mPlaylists.at(i) );
   }
}


void MainWidget::handleAdd()
{
   int i;
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::AnyFile );
   fileDialog.setAcceptMode( QFileDialog::AcceptSave );
   fileDialog.setConfirmOverwrite( false );
   fileDialog.setDefaultSuffix( "m3u" );
//   fileDialog.setDirectory( QDir( mpM3uFileName->text() ).absolutePath() );
   fileDialog.setWindowTitle( tr("Select Karma Playlist") );
   fileDialog.setFilter( "Playlist (*.m3u)" );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      for( i = 0; i < fileDialog.selectedFiles().count() ; i++ )
      {
         if( mPlaylists.indexOf( fileDialog.selectedFiles().at(i) ) < 0 )
         mPlaylists.append( fileDialog.selectedFiles().at(i) );
      }
      mPlaylists.sort();
      MySettings().setValue( "Playlists", mPlaylists );
      
      updateLists();
   }
}


void MainWidget::handleRemove( QAction *action )
{
   mPlaylists.removeAll( action->toolTip() );
   
   MySettings().setValue( "Playlists", mPlaylists );
   updateLists();
}