/*
 * src/apps/Underpass/UnderpassMainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "UnderpassMainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QAction>
#include <QComboBox>
#include <QCommonStyle>
#include <QDateTime>
#include <QDragEnterEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QTimer>

/* local library headers */
#include <CrashCleanup.hpp>
#include <GenericSatelliteHandler.hpp>
#include <GlobalConfigWidget.hpp>
#include <ProxyWidget.hpp>
#include <Settings.hpp>
#include <Satellite.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "UnderpassConfigDialog.hpp"


UnderpassMainWidget::UnderpassMainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get() )
, mpGenericSatelliteHandler( 0 )
, mpConfig( new UnderpassConfigDialog( this ) )
, mpProcess( new QProcess( this ) )
, mpAddAction( new QAction( tr("Add"), this ) )
, mpDeleteAction( new QAction( tr("Delete"), this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpAddDeleteButton( new QPushButton( tr("Add/Delete"), this ) )
, mpStartButton( new QPushButton( QCommonStyle().standardIcon(QStyle::SP_MediaPlay), tr("Start"), this ) )
, mpStation( new QComboBox( this ) )
, mpUrl( new QLineEdit( this ) )
, mpPlayer( new QComboBox( this ) )
, mpMessageBuffer( new QListWidget( this ) )
{
   QMenu *addDeleteMenu = new QMenu( mpAddDeleteButton );
   addDeleteMenu->addAction( mpAddAction );
   addDeleteMenu->addAction( mpDeleteAction );
   mpAddDeleteButton->setMenu( addDeleteMenu );

   mpStartButton->setCheckable( true );
   const QString stationsPrefix( "Stations/" );
   foreach( const QString &entry, Settings::get()->allKeys() )
   {
      if( entry.startsWith( stationsPrefix ) )
      {
         mpStation->addItem( entry.mid( stationsPrefix.size() ) );
      }
   }

   QStringList players( Settings::value( Settings::UnderpassPlayers ) );
   /* temporary until settings dialog takes care of this */
   Settings::setValue( Settings::UnderpassPlayers, players );
   mpPlayer->addItems( players );

   CrashCleanup::addObject( mpProcess );
   QGridLayout *mainLayout   = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->setColumnStretch( 1, 1 );
   parent->setWindowIcon( QIcon( ":/Underpass/Icon.png" ) );

   mainLayout->addWidget( new QLabel( tr( "Station:" ), this ), 0, 0 );
   mainLayout->addWidget( new QLabel( tr( "Url:"), this ),      1, 0 );
   mainLayout->addWidget( new QLabel( tr( "Player:"), this ),   2, 0 );
   mainLayout->addWidget( mpStation,        0, 1 );
   mainLayout->addWidget( mpAddDeleteButton,0, 2 );
   mainLayout->addWidget( mpUrl,            1, 1, 1, 2 );
   mainLayout->addWidget( mpPlayer,         2, 1 );
   mainLayout->addWidget( mpStartButton,    2, 2 );
   mainLayout->addWidget( mpMessageBuffer,  4, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton, 5, 0, 1, 3 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );

   readConfig();

   setLayout( mainLayout );
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   setAcceptDrops( true );

   if( mpSatellite )
   {
      mpGenericSatelliteHandler = new GenericSatelliteHandler( mpSatellite, GenericSatelliteHandler::WithPingAndDialog, this );
      connect( mpGenericSatelliteHandler, SIGNAL(updateConfig()),
               mpConfig, SLOT(readSettings()) );
      connect( mpSatellite, SIGNAL(received(QByteArray)),
               this, SLOT(handleSatellite(QByteArray)) );
   }
   connect( mpStation, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(readData(QString)) );
   connect( mpStartButton, SIGNAL(clicked(bool)),
            this, SLOT(startProcess(bool)) );
   connect( mpProcess, SIGNAL(readyReadStandardError()),
            this, SLOT(readProcessOutput()) );

   int index = mpStation->findText( Settings::value( Settings::UnderpassLastStation ) );
   if( index >= 0 )
   {
      mpStation->setCurrentIndex( index );
      readData( Settings::value( Settings::UnderpassLastStation ) );
   }

   WidgetShot::addWidget( "Main", this );
}


UnderpassMainWidget::~UnderpassMainWidget()
{
   Settings::setValue( Settings::UnderpassLastStation, mpStation->currentText() );
}


void UnderpassMainWidget::readConfig()
{
   //! \todo replace or remove
   //mpSatellite->restart();
}


void UnderpassMainWidget::handleSatellite( const QByteArray &msg )
{
   QStringList src( Satellite::split( msg ) );

   if( src.at(0) == "p0p" )
   {
      if( Settings::value( Settings::UnderpassStopOnPartymanStart ) )
      {
         startProcess( false );
      }
   }
   else if( src.at(0) == "U0S" )
   {
      startProcess( false );
   }
   else if( src.at(0) == "U0P" )
   {
      startProcess( true );
   }
   else if( src.at(0) == "U0N" )
   {
      // switch to next station
      int index = mpStation->currentIndex() + 1;
      if( index >= mpStation->count() )
      {
         index = 0;
      }
      mpStation->setCurrentIndex( index );
   }
}


void UnderpassMainWidget::readData( const QString &name )
{
   QSettings *settings = Settings::get();

   QString data( settings->value(QString("Stations/%1").arg(name) ).toString() );
   int colonPos = data.indexOf(':');
   if( colonPos >= 0 )
   {
      mpUrl->setText( data.mid( colonPos + 1 ) );
      int index = mpPlayer->findText( data.left( colonPos - 1 ) );
      if( index >= 0 )
      {
         mpPlayer->setCurrentIndex( index );
      }
   }
   if( mpStartButton->isChecked() )
   {
      startProcess( false );
      startProcess( true );
   }
}


void UnderpassMainWidget::startProcess( bool start )
{
   mpStartButton->setChecked( start );
   mpAddDeleteButton->setDisabled( start );
   mpPlayer->setDisabled( start );
   mpUrl->setDisabled( start );
   mpStartButton->setIcon( start ? QCommonStyle().standardIcon(QStyle::SP_MediaStop) :
                                   QCommonStyle().standardIcon(QStyle::SP_MediaPlay) );
   mpStartButton->setText( start ? tr("Stop") : tr("Start") );
   mpSatellite->send( start ? "u0p" : "u0s" );
   if( start )
   {
      ProxyWidget::setProxy( mpProcess );
      QStringList args;
#if 1
      args << "-v" << mpUrl->text();
      QStringList environment( mpProcess->environment() );
#else
      args << "-c" << mpPlayer->currentText() + " -v " + mpUrl->text();
      mpProcess->start( "/bin/sh", args );
#endif
      mpProcess->setEnvironment( environment );
      mpProcess->start( mpPlayer->currentText(), args );
   }
   else
   {
      mpProcess->terminate();
   }
}


void UnderpassMainWidget::readProcessOutput()
{
   QStringList output( QString::fromLocal8Bit(mpProcess->readAllStandardError().constData()).split('\n') );

   foreach( const QString &line, output )
   {
      if( line.startsWith('\r') && mpMessageBuffer->count() > 0 )
      {
         QListWidgetItem *item = mpMessageBuffer->takeItem(mpMessageBuffer->count() - 1);
         if( item )
         {
            delete item;
         }
         mpMessageBuffer->addItem( line.mid(1) );
      }
      else
      {
         mpMessageBuffer->addItem( line );
      }
   }

   while( mpMessageBuffer->count() > 200 )
   {
      QListWidgetItem *item = mpMessageBuffer->takeItem(0);
      if( item )
      {
         delete item;
      }
   }

   mpMessageBuffer->scrollToBottom();
}


void UnderpassMainWidget::dragEnterEvent( QDragEnterEvent *event )
{
   event->acceptProposedAction();
}


void UnderpassMainWidget::dropEvent( QDropEvent *event )
{
#if 0
   mpDropDialog->dropEvent( event );
   mpDropDialog->show();
#endif
}
