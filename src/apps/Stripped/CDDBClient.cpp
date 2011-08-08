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
#include <QStringList>
#include <QTimer>

/* local library headers */
#include <MySettings.hpp>
#include <ProxyWidget.hpp>

/* local headers */
#include "CDInfo.hpp"
#include "ConfigDialog.hpp"


CDDBClient::CDDBClient( CDInfo *cdinfo, QWidget *parent )
: QWidget( parent )
, mpCount( new QLabel( this ) )
, mpHits( new QComboBox( this ) )
, mpSplitLabel( new QLabel( tr("Split:"), this ) )
, mpSplit( new QComboBox( this ) )
, mpNAM( new QNetworkAccessManager( this ) )
, mpCDInfo( cdinfo )
, mQueryReplies()
, mReadReplies()
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


   connect( mpHits, SIGNAL(activated(int)),
            this, SLOT(handleComboBox(int)) );
   connect( mpSplit, SIGNAL(activated(const QString &)),
            this, SLOT(handleSplit(const QString &)) );
   connect( mpNAM, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleQueryData(QNetworkReply*)) );
   connect( mpNAM, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleReadData(QNetworkReply*)) );

   clear();
   setLayout( mainLayout );
}


CDDBClient::~CDDBClient()
{
}


void CDDBClient::clear()
{
   mpHits->clear();
   mpHits->addItem( mReadCDTextMessage );
   mpHits->addItem( mQueryCDDBMessage );
   mpHits->insertSeparator( mpHits->count() );
   mpHits->setCurrentIndex( mpHits->findText( mQueryCDDBMessage) );
   mpCount->setText( "0:" );
}


void CDDBClient::cancel()
{
   QNetworkReply *reply = 0;
   while( mQueryReplies.size() )
   {
      reply = mQueryReplies.takeFirst();
      reply->close();
      reply->deleteLater();
   }
   while( mReadReplies.size() )
   {
      reply = mReadReplies.takeFirst();
      reply->close();
      reply->deleteLater();
   }
}


void CDDBClient::handleSplit( const QString &token )
{
   mpCDInfo->splitTitle( token.isEmpty() ? mpSplit->currentText() : token );
   emit infoUpdated();
}


void CDDBClient::handleComboBox( int index )
{
   if( (index < 0) || (index > mpHits->count()) )
   {
      return;
   }
   /* make sure that combo box shows right entry, even if called from code */
   mpHits->setCurrentIndex( index );
   if( index == mpHits->findText( mReadCDTextMessage ) )
   {
      emit requestCDText();
   }
   else if( index == mpHits->findText( mQueryCDDBMessage ) )
   {
      /* request from FreeDB */
      if( !mpCDInfo->tracks() )
      {
         emit stateNet();
         return;
      }
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

      emit message( tr("Querying FreeDB") );
      startRequest( "query", parameters );
   }
   else
   {
      QStringList parameters( mpHits->currentText().split( ' ', QString::SkipEmptyParts ) );
      if( parameters.size() > 2 )
      {
         while( parameters.size() > 2 )
         {
            parameters.removeLast();
         }
         emit message( tr("Reading data from FreeDB") );
         emit stateNet();
         startRequest( "read", parameters );
      }
   }
}


void CDDBClient::startRequest( const QString &cmd, const QStringList &parameter )
{
   if( cmd.isEmpty() )
   {
      return;
   }
   /* TODO: get useful name */
   QString url( "http://freedb.freedb.org/~cddb/cddb.cgi?cmd=cddb %1 %2&hello=svolli localhost Stripped alpha&proto=6" );

   QNetworkRequest request( QUrl( url.arg( cmd, parameter.join(" ") ).replace( ' ', '+' ) ) );
   ProxyWidget::setProxy( mpNAM );
   if( cmd == "query" )
   {
      mQueryReplies.append( mpNAM->get( request ) );
   }
   else if( cmd == "read" )
   {
      mReadReplies.append( mpNAM->get( request ) );
   }
   else
   {
      Q_ASSERT_X( false, "CDDBClient::startRequest",
                  QString("undefined command: %1").arg(cmd).toLocal8Bit().constData() );
   }
}


void CDDBClient::handleQueryData( QNetworkReply *reply )
{
   if( !mQueryReplies.contains( reply ) )
   {
      return;
   }
   /* remove reply from cancel-list and mark it for deletion */
   mQueryReplies.takeAt( mQueryReplies.indexOf( reply ) )->deleteLater();
   if( reply->error() != QNetworkReply::NoError )
   {
      return;
   }
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
         for( int i = -1; i < 100; i++ )
         {
            mpCDInfo->setTitle( i, QString(), true );
         }
         emit stateDisc();
         emit infoUpdated();
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
   QTimer::singleShot( 1500, this, SIGNAL( message() ) );
   mpCount->setText( QString::number( mpHits->count() - 3 ) + ":" );
   if( MySettings().VALUE_AUTOFREEDB && (mpHits->count() > 3) )
   {
      handleComboBox( 3 );
   }
   else
   {
      emit stateDisc();
   }
}


void CDDBClient::handleReadData( QNetworkReply *reply )
{
   if( !mReadReplies.contains( reply ) )
   {
      return;
   }
   /* remove reply from cancel-list and mark it for deletion */
   mReadReplies.takeAt( mReadReplies.indexOf( reply ) )->deleteLater();
   if( reply->error() != QNetworkReply::NoError )
   {
      return;
   }
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
   QTimer::singleShot( 1500, this, SIGNAL( message() ) );

   emit stateDisc();
   handleSplit();
}

