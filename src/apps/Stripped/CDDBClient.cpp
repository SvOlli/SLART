/**
 * src/apps/Stripped/CDDBClient.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDDBClient.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStateMachine>
#include <QStringList>
#include <QTimer>

/* local library headers */
#include <Settings.hpp>
#include <ProxyWidget.hpp>

/* local headers */
#include "CDInfo.hpp"

#include <Trace.hpp>

CDDBClient::CDDBClient( CDInfo *cdinfo, QWidget *parent )
: QWidget( parent )
, mRequestType( NoRequest )
, mpStateMachine( new QStateMachine( this ) )
, mpStateEjected( new QState( mpStateMachine ) )
, mpStateCleared( new QState( mpStateMachine ) )
, mpStateQuery( new QState( mpStateMachine ) )
, mpStateRead( new QState( mpStateMachine ) )
, mpStateCDText( new QState( mpStateMachine ) )
, mpStateDone( new QState( mpStateMachine ) )
, mpCount( new QLabel( this ) )
, mpHits( new QComboBox( this ) )
, mpSplitLabel( new QLabel( tr("Split:"), this ) )
, mpSplit( new QComboBox( this ) )
, mpNAM( new QNetworkAccessManager( this ) )
, mpCDInfo( cdinfo )
, mpMessageResetTimer( new QTimer( this ) )
, mReadCDTextMessage( tr("Read CD-Text") )
, mQueryCDDBMessage( tr("Query FreeDB") )
{
   QHBoxLayout *mainLayout = new QHBoxLayout( this );

   mainLayout->setContentsMargins( 0, 0, 0, 0 );

   mpSplit->addItem( " / " );
   mpSplit->addItem( " - " );
   mpSplit->addItem( "/" );
   mpSplit->addItem( "-" );

   mainLayout->addWidget( mpCount );
   mainLayout->addWidget( mpHits );
   mainLayout->addWidget( mpSplitLabel );
   mainLayout->addWidget( mpSplit );
   mainLayout->setStretchFactor( mpCount,      0 );
   mainLayout->setStretchFactor( mpHits,       1 );
   mainLayout->setStretchFactor( mpSplitLabel, 0 );
   mainLayout->setStretchFactor( mpSplit,      0 );

   /* setting up the state machine and states */
   mpStateMachine->setInitialState( mpStateEjected );

   mpStateEjected->assignProperty( this, "statusMessage", tr("ejected") );
   mpStateEjected->assignProperty( mpHits, "enabled", false );
   mpStateEjected->assignProperty( mpSplit, "enabled", false );
   mpStateEjected->addTransition( this, SIGNAL(cdinsert()), mpStateCleared );
   connect( mpStateEjected, SIGNAL(entered()),
            this, SLOT(handleStateEjected()) );

   mpStateCleared->assignProperty( this, "statusMessage", tr("cleared") );
   mpStateCleared->assignProperty( mpHits, "enabled", false );
   mpStateCleared->assignProperty( mpSplit, "enabled", false );
   mpStateCleared->addTransition( this, SIGNAL(automatic()), mpStateQuery );
   mpStateCleared->addTransition( this, SIGNAL(eject()), mpStateEjected );
   mpStateCleared->addTransition( this, SIGNAL(found()), mpStateDone );
   connect( mpStateCleared, SIGNAL(entered()),
            this, SLOT(handleStateCleared()) );

   mpStateQuery->assignProperty( this, "statusMessage", tr("query") );
   mpStateQuery->assignProperty( mpHits, "enabled", false );
   mpStateQuery->assignProperty( mpSplit, "enabled", false );
   mpStateQuery->addTransition( this, SIGNAL(cdinsert()), mpStateCleared );
   mpStateQuery->addTransition( this, SIGNAL(found()), mpStateDone );
   mpStateQuery->addTransition( this, SIGNAL(automatic()), mpStateRead );
   mpStateQuery->addTransition( this, SIGNAL(eject()), mpStateEjected );
   connect( mpStateQuery, SIGNAL(entered()),
            this, SLOT(handleStateQuery()) );

   mpStateRead->assignProperty( this, "statusMessage", tr("read") );
   mpStateRead->assignProperty( mpHits, "enabled", false );
   mpStateRead->assignProperty( mpSplit, "enabled", false );
   mpStateRead->addTransition( this, SIGNAL(cdinsert()), mpStateCleared );
   mpStateRead->addTransition( this, SIGNAL(gotdata()), mpStateDone );
   mpStateRead->addTransition( this, SIGNAL(eject()), mpStateEjected );
   connect( mpStateRead, SIGNAL(entered()),
            this, SLOT(handleStateRead()) );

   mpStateCDText->assignProperty( this, "statusMessage", tr("cdtext") );
   mpStateCDText->assignProperty( mpHits, "enabled", false );
   mpStateCDText->assignProperty( mpSplit, "enabled", false );
   mpStateCDText->addTransition( this, SIGNAL(cdinsert()), mpStateCleared );
   mpStateCDText->addTransition( this, SIGNAL(gotdata()), mpStateDone );
   mpStateCDText->addTransition( this, SIGNAL(eject()), mpStateEjected );
   connect( mpStateCDText, SIGNAL(entered()),
            this, SLOT(handleStateCDText()) );

   mpStateDone->assignProperty( this, "statusMessage", tr("done") );
   mpStateDone->assignProperty( mpHits, "enabled", true );
   mpStateDone->assignProperty( mpSplit, "enabled", true );
   mpStateDone->addTransition( this, SIGNAL(cdinsert()), mpStateCleared );
   mpStateDone->addTransition( this, SIGNAL(query()), mpStateQuery );
   mpStateDone->addTransition( this, SIGNAL(cdtext()), mpStateCDText );
   mpStateDone->addTransition( this, SIGNAL(select()), mpStateRead );
   mpStateDone->addTransition( this, SIGNAL(eject()), mpStateEjected );
   connect( mpStateDone, SIGNAL(entered()),
            this, SLOT(handleStateDone()) );

   mpMessageResetTimer->setInterval( 1500 );
   mpMessageResetTimer->setSingleShot( true );
   connect( mpMessageResetTimer, SIGNAL(timeout()),
            this, SIGNAL(message()) );

   connect( mpHits, SIGNAL(activated(int)),
            this, SLOT(handleComboBox()) );
   connect( mpSplit, SIGNAL(activated(const QString &)),
            this, SLOT(handleSplit(const QString &)) );
   connect( mpNAM, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleServerReply(QNetworkReply*)) );

   clear();
   setLayout( mainLayout );

   mpStateMachine->start();
}


CDDBClient::~CDDBClient()
{
}


CDDBClient::RequestType CDDBClient::requestType() const
{
   return mRequestType;
}


void CDDBClient::setRequestType( RequestType value )
{
   mRequestType = value;
}


QString CDDBClient::statusMessage() const
{
   return mStatusMessage;
}


void CDDBClient::setStatusMessage( const QString &value )
{
   mStatusMessage = value;
   emit message( mStatusMessage );
   qDebug() << value;
}


void CDDBClient::handleStateEjected()
{
   mpMessageResetTimer->start();
}


void CDDBClient::handleStateCleared()
{
   clear();
   if( Settings::value( Settings::StrippedAutoFreeDB ) )
   {
      emit automatic();
   }
   else
   {
      emit found();
   }
   mpMessageResetTimer->start();
}


void CDDBClient::handleStateQuery()
{
   emit stateNet();
   QStringList parameters;
   parameters.append( mpCDInfo->cddbDiscID() );
   parameters.append( QString::number( mpCDInfo->tracks() ) );
   for( int i = 1; i <= mpCDInfo->tracks(); i++ )
   {
      parameters.append( QString::number( 150 + mpCDInfo->firstSector(i) ) );
   }
   parameters.append( QString::number( ( mpCDInfo->lastSector(-1) -
                                          mpCDInfo->firstSector(-1) )
                                        / 75 + 2 ) );

   startRequest( RequestQuery, parameters );

   mpMessageResetTimer->start();
}


void CDDBClient::handleStateRead()
{
   emit stateNet();
   QStringList parameters( mpHits->currentText().split( ' ', QString::SkipEmptyParts ) );
   if( parameters.size() > 2 )
   {
      while( parameters.size() > 2 )
      {
         parameters.removeLast();
      }
      startRequest( RequestTracks, parameters );
   }
   mpMessageResetTimer->start();
}


void CDDBClient::handleStateCDText()
{
   emit requestCDText();
   mpMessageResetTimer->start();
}


void CDDBClient::handleStateDone()
{
   mRequestType = NoRequest;
   mpMessageResetTimer->start();
   emit stateDisc();
}


void CDDBClient::clear()
{
   int index = mpHits->currentIndex();
   mpHits->clear();
   mpHits->addItem( mReadCDTextMessage );
   mpHits->addItem( mQueryCDDBMessage );
   mpHits->insertSeparator( mpHits->count() );
   if( (index < 0) || (index > mpHits->findText( mQueryCDDBMessage )) )
   {
      index = mpHits->findText( mQueryCDDBMessage );
   }
   mpHits->setCurrentIndex( index );
   mpCount->setText( "0:" );
}


void CDDBClient::handleSplit( const QString &token )
{
   mpCDInfo->splitTitle( token.isEmpty() ? mpSplit->currentText() : token );
}


void CDDBClient::handleComboBox()
{
   int index = mpHits->currentIndex();

   if( !mpCDInfo->tracks() )
   {
      return;
   }

   if( index == mpHits->findText( mReadCDTextMessage ) )
   {
      emit cdtext();
   }
   else if( index == mpHits->findText( mQueryCDDBMessage ) )
   {
      emit query();
   }
   else
   {
      emit select();
   }
}


void CDDBClient::startRequest( RequestType type, const QStringList &parameter )
{
   QString cmd;
   switch( type )
   {
   case RequestQuery:
      cmd = "query";
      break;
   case RequestTracks:
      cmd = "read";
      break;
   default:
      qFatal( "CDDBClient::startRequest: illegal RequestType" );
   }
   mRequestType = type;

   QString url( "http://freedb.freedb.org/~cddb/cddb.cgi?cmd=cddb %1 %2&hello=svolli localhost Stripped alpha&proto=6" );

   QNetworkRequest request( QUrl( url.arg( cmd, parameter.join(" ") ).replace( ' ', '+' ) ) );
   ProxyWidget::setProxy( mpNAM );
   mpNAM->get( request );
}


void CDDBClient::handleServerReply( QNetworkReply *reply )
{
   reply->deleteLater();
   if( reply->error() != QNetworkReply::NoError )
   {
      return;
   }
   switch( mRequestType )
   {
   case RequestQuery:
      handleQueryData( reply );
      break;
   case RequestTracks:
      handleReadData( reply );
      break;
   default:
      break;
   }
}


void CDDBClient::handleQueryData( QNetworkReply *reply )
{
   QString data( QString::fromUtf8( reply->readAll().constData() ) );
   data.remove( '\r' );
   QStringList response( data.split( '\n', QString::SkipEmptyParts ) );

   QStringList status( response.at(0).split(' ', QString::SkipEmptyParts) );

   clear();
   switch( status.at(0).toULong() )
   {
      case 200:
         /* got one */
         mpHits->addItem( response.at(0).mid(4) );
         break;
      case 202:
         /* found none */
         break;
      case 210:
      case 211:
         /* got many */
         {
            /* sort the entry starting with the third field */
            QStringList list;
            for( int i = 1; i < response.size(); i++ )
            {
               if( response.at(i).size() > 1 )
               {
                  QStringList entry( response.at(i).split(" ") );
                  entry.append( entry.takeFirst() );
                  entry.append( entry.takeFirst() );
                  list << entry.join(" ");
               }
            }
            list.sort();
            for( int i = 0; i < list.size(); i++ )
            {
               QStringList entry( list.at(i).split(" ") );
               entry.prepend( entry.takeLast() );
               entry.prepend( entry.takeLast() );
               mpHits->addItem( entry.join(" ") );
            }
         }
         break;
      default:
         break;
   }
   mpCount->setText( QString::number( mpHits->count() - 3 ) + ":" );

   mpHits->setCurrentIndex( (mpHits->count() > 3) ? 3 : 1 );
   if( Settings::value( Settings::StrippedAutoFreeDB ) &&
       (mpHits->count() > 3) )
   {
      emit automatic();
   }
   else
   {
      emit found();
   }
}


void CDDBClient::handleReadData( QNetworkReply *reply )
{
   QString data( QString::fromUtf8( reply->readAll().constData() ) );
   data.remove( '\r' );
   QStringList response( data.split( '\n', QString::SkipEmptyParts ) );
   QString line;

   long currentTrack = -1;
   mpCDInfo->setTitle();
   mpCDInfo->setGenre();

   for( int i = 1; i < response.size(); i++ )
   {
      line = response.at(i);

      if( line.startsWith( "DTITLE" ) )
      {
         mpCDInfo->setTitle( -1, line.mid( line.indexOf('=')+1 ), true );
      }

      if( line.startsWith( "DYEAR" ) )
      {
         bool ok;
         mpCDInfo->setYear( line.mid( line.indexOf('=')+1 ).toInt( &ok ) );
         if( !ok )
         {
            mpCDInfo->setYear( -1 );
         }
      }

      if( line.startsWith( "DGENRE" ) )
      {
         mpCDInfo->setGenre( line.mid( line.indexOf('=')+1 ), true );
      }

      if( line.startsWith( "TTITLE" ) )
      {
         long tracknr = line.mid( 6, line.indexOf('=')-6 ).toLong();
         mpCDInfo->setTitle( tracknr+1, line.mid( line.indexOf('=')+1 ),
                             (tracknr == currentTrack) );
         currentTrack = tracknr;
      }
   }

   handleSplit();
   emit gotdata();
}
