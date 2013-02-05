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
#include <QDragEnterEvent>
#include <QGridLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QProcess>
#include <QPushButton>
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
#include "StationStorage.hpp"
#include "UnderpassConfigDialog.hpp"


UnderpassMainWidget::UnderpassMainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get() )
, mpStorage( new StationStorage( this ) )
, mpGenericSatelliteHandler( 0 )
, mpConfig( new UnderpassConfigDialog( mpStorage, this ) )
, mpProcess( new QProcess( this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpAddStationButton( new QPushButton( tr("Add"), this ) )
, mpStartButton( new QPushButton( QCommonStyle().standardIcon(QStyle::SP_MediaPlay), tr("Start"), this ) )
, mpStation( new QComboBox( this ) )
, mpUrl( new QLineEdit( this ) )
, mpPlayer( new QComboBox( this ) )
, mpMessageBuffer( new QListWidget( this ) )
, mDeleteLastLine( false )
, mLineBuffer()
, mLastStation()
{
   mpMessageBuffer->setFont( QFont( "Courier") );
   mpStartButton->setCheckable( true );
   mpAddStationButton->setToolTip( tr("click to add a new station\nclear url to remove current station") );
   QStringList stationList( mpStorage->stationList() );
   QStringList playerList( mpStorage->playerList() );
   if( stationList.isEmpty() )
   {
      mpStorage->createDefaults();
      stationList = mpStorage->stationList();
   }
   stationList.sort();
   playerList.sort();
   mpStation->addItems( stationList );
   mpPlayer->addItems( playerList );

   CrashCleanup::addObject( mpProcess );
   QGridLayout *mainLayout   = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->setColumnStretch( 1, 1 );
   parent->setWindowIcon( QIcon( ":/Underpass/Icon.png" ) );

   mainLayout->addWidget( new QLabel( tr( "Station:" ), this ), 0, 0 );
   mainLayout->addWidget( new QLabel( tr( "Url:"), this ),      1, 0 );
   mainLayout->addWidget( new QLabel( tr( "Player:"), this ),   2, 0 );
   mainLayout->addWidget( mpStation,          0, 1 );
   mainLayout->addWidget( mpAddStationButton, 0, 2 );
   mainLayout->addWidget( mpUrl,              1, 1, 1, 2 );
   mainLayout->addWidget( mpPlayer,           2, 1 );
   mainLayout->addWidget( mpStartButton,      2, 2 );
   mainLayout->addWidget( mpMessageBuffer,    4, 0, 1, 3 );
   mainLayout->addWidget( mpSettingsButton,   5, 0, 1, 3 );

   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );
   connect( mpConfig, SIGNAL(configChanged()),
            this, SLOT(readConfig()) );
   connect( mpStation, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(handleStationChange(QString)) );
   connect( mpAddStationButton, SIGNAL(clicked()),
            this, SLOT(addStation()) );
   connect( mpStartButton, SIGNAL(clicked(bool)),
            this, SLOT(startProcess(bool)) );
   connect( mpProcess, SIGNAL(readyReadStandardError()),
            this, SLOT(readProcessOutput()) );
   connect( mpProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(processError(QProcess::ProcessError)) );
   connect( mpProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(processFinished(int,QProcess::ExitStatus)) );
   connect( mpUrl, SIGNAL(textChanged(QString)),
            mpStorage, SLOT(setUrl(QString)), Qt::QueuedConnection );
   connect( mpPlayer, SIGNAL(currentIndexChanged(QString)),
            mpStorage, SLOT(setPlayer(QString)), Qt::QueuedConnection );

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

   const QString &lastStation( Settings::value( Settings::UnderpassLastStation ) );
   int index = stationList.indexOf( lastStation );
   if( index >= 0 )
   {
      mpStation->setCurrentIndex( index );
      handleStationChange( lastStation );
   }
   WidgetShot::addWidget( "Main", this );
}


UnderpassMainWidget::~UnderpassMainWidget()
{
   CrashCleanup::removeObject( mpProcess );
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


void UnderpassMainWidget::handleStationChange( const QString &name )
{
   if( !mLastStation.isEmpty() && mpUrl->text().isEmpty() )
   {
      mpStorage->removeStation( mLastStation );
      mLastStation.clear();
   }
   else
   {
      mLastStation = name;
   }
   mpStorage->setStation( name );
   mpUrl->setText( mpStorage->url() );
   setComboBoxByValue( mpPlayer, mpStorage->player() );
   if( mpStorage->stationListChanged() )
   {
      updateStationList();
   }
   if( mpStartButton->isChecked() )
   {
      startProcess( false );
      QTimer::singleShot( 400, this, SLOT(startProcess()) );
   }
}


void UnderpassMainWidget::updateStationList()
{
   const QString stationName( mpStation->currentText() );
   mpStation->clear();
   QStringList stationList( mpStorage->stationList() );
   stationList.sort();
   mpStation->addItems( stationList );
   setComboBoxByValue( mpStation, stationName );
}


void UnderpassMainWidget::addStation()
{
   bool ok;
   QString text = QInputDialog::getText( this, tr("Add Station:"),
                                         tr("Add Station:"),
                                         QLineEdit::Normal, QString(), &ok );
   if( ok && !text.isEmpty() )
   {
      mpStorage->addStation( text, mpPlayer->currentText(), mpUrl->text() );
      updateStationList();
      setComboBoxByValue( mpStation, text );
   }
}


void UnderpassMainWidget::startProcess( bool start )
{
   mpStartButton->setChecked( start );
   mpAddStationButton->setDisabled( start );
   mpPlayer->setDisabled( start );
   mpUrl->setDisabled( start );
#if 0
   mpStartButton->setIcon( start ? QCommonStyle().standardIcon(QStyle::SP_MediaStop) :
                                   QCommonStyle().standardIcon(QStyle::SP_MediaPlay) );
   mpStartButton->setText( start ? tr("Stop") : tr("Start") );
#endif
   mpSatellite->send( start ? "u0p" : "u0s" );
   if( start )
   {
      ProxyWidget::setProxy( mpProcess );
      QStringList args;
#if 1
      args << "-v" << mpUrl->text();
      systemMessage( QString("%1 %2").arg(mpPlayer->currentText(),args.join(" ")) );
      mpProcess->start( mpPlayer->currentText(), args );
#else
      args << "-c" << mpPlayer->currentText() + " -v " + mpUrl->text();
      mpProcess->start( "/bin/sh", args );
#endif
   }
   else
   {
      mpProcess->terminate();
   }
}


void UnderpassMainWidget::systemMessage( const QString &text )
{
   QListWidgetItem *item = new QListWidgetItem( text, mpMessageBuffer );
   item->setBackground( QBrush( mpMessageBuffer->palette().color( QPalette::AlternateBase ) ) );
   mpMessageBuffer->addItem( item );
   mpMessageBuffer->scrollToBottom();
}


void UnderpassMainWidget::readProcessOutput()
{
   QString output( QString::fromLocal8Bit( mpProcess->readAllStandardError().constData() ) );

   QListWidgetItem *item = 0;
   foreach( const QChar &qc, output )
   {
      char c = qc.toAscii();
      if( (c == '\r') || (c == '\n') )
      {
         if( mDeleteLastLine )
         {
            for( int i = 1; (item = mpMessageBuffer->item( mpMessageBuffer->count() - i )) ; ++i )
            {
               if( item->background() != QBrush( mpMessageBuffer->palette().color( QPalette::AlternateBase ) ) )
               {
                  item->setText( mLineBuffer );
                  break;
               }
            }
         }
         else
         {
            mpMessageBuffer->addItem( mLineBuffer );
         }
         while( mpMessageBuffer->count() > Settings::value( Settings::UnderpassBufferSize ) )
         {
            item = mpMessageBuffer->takeItem( 0 );
            if( item )
            {
               delete item;
            }
         }
         mLineBuffer.clear();
         if( mpMessageBuffer->visualItemRect( mpMessageBuffer->item( mpMessageBuffer->count() - 1 ) ).y()
               < mpMessageBuffer->size().height() )
         {
            mpMessageBuffer->scrollToBottom();
         }
         mDeleteLastLine = (c == '\r');
      }
      else if( c == '\t' )
      {
         mLineBuffer += QString(' ').repeated( 8 - mLineBuffer.size() % 8 );
      }
      else if( !c || (c >= ' ') )
      {
         mLineBuffer += qc;
      }
   }
}


void UnderpassMainWidget::setPlayerText( const QString &text )
{
   setComboBoxByValue( mpPlayer, text );
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


void UnderpassMainWidget::sortComboBox( QComboBox *comboBox )
{
   QStringList data;
   for( int i = 0; i < comboBox->count(); ++i )
   {
      data << comboBox->itemText( i );
   }
   data.sort();
   comboBox->clear();
   comboBox->addItems( data );
}


void UnderpassMainWidget::setComboBoxByValue( QComboBox *comboBox, const QString &value )
{
   int index = comboBox->findText( value );
   if( index >= 0 )
   {
      comboBox->setCurrentIndex( index );
   }
}


void UnderpassMainWidget::processError( QProcess::ProcessError error )
{
   if( error == QProcess::FailedToStart )
   {
      systemMessage( tr("process failed to start, programm missing?") );
      startProcess( false );
   }
   else
   {
      if( mpStartButton->isChecked() )
      {
         QTimer::singleShot( 0, this, SLOT(startProcess()) );
      }
   }
}
