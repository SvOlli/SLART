/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "ConfigDialog.hpp"
#include "ControlWidget.hpp"
#include "PlaylistWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpParent( parent )
, mpConfig( new ConfigDialog( this ) )
, mpPlaylist( new PlaylistWidget( mpConfig, this ) )
, mpControl( new ControlWidget( mpConfig, mpPlaylist, this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   mainLayout->addWidget( mpControl );
   mainLayout->addWidget( mpPlaylist );
   mpParent->setAttribute( Qt::WA_AlwaysShowToolTips, true );
   mpParent->setWindowIcon( QIcon( ":/PartymanSmile.gif" ) );


   setLayout( mainLayout );
   
   connect( mpControl, SIGNAL(requestAddToPlaylist(QStringList,bool)), 
            mpPlaylist, SLOT(addEntries(QStringList,bool)) );
   connect( mpControl, SIGNAL(requestChangeTitle(QIcon,QString)),
            this, SLOT(changeTitle(QIcon,QString)) );
   connect( mpPlaylist, SIGNAL(playlistIsValid(bool)), mpControl, SLOT(allowConnect(bool)) );
   connect( mpConfig, SIGNAL(configChanged()), mpPlaylist, SLOT(readM3u()) );
}


void MainWidget::changeTitle( const QIcon &icon, const QString &title )
{
   emit requestChangeTitle( icon, title );
}


void MainWidget::startUp()
{
   MySettings settings;
   mpPlaylist->readM3u();
   mpControl->readConfig();
   if( !QFileInfo( settings.value( "DatabaseFilename", "" ).toString() ).isFile() )
   {
      mpConfig->exec();
   }
   if( settings.value( "AutoConnect", false ).toBool() )
   {
      mpControl->initConnect();
   }
   else
   {
      mpControl->initDisconnect();
   }
}
