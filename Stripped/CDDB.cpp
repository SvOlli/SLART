/**
 * CDDB.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "CDDB.hpp"
#include "CDToc.hpp"

#include <QtGui>
#include <QHttp>

#include "Trace.hpp"


CDDB::CDDB( CDToc *toc, QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpToc( toc )
, mpCount( new QLabel( this ) )
, mpHits( new QComboBox( this ) )
, mHttpGetId( 0 )
, mpHttp( new QHttp( "freedb.freedb.org", 80, this ) )
, mpFile( new QFile( this ) )
, mMutex()
{
   QHBoxLayout *mainLayout = new QHBoxLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 0 );
#else
   mainLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
   
   mpCount->setText( "0:" );
   mpHits->addItem( tr("CDDB/freedb client") );
   
   mpFile->setFileName( "freedb.reply" );
   
   mainLayout->addWidget( mpCount );
   mainLayout->addWidget( mpHits );
   mainLayout->setStretchFactor( mpCount, 0 );
   mainLayout->setStretchFactor( mpHits,  1 );
   
   setLayout( mainLayout );
   
   connect( mpHttp, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)) );
   connect( mpHits, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(read(QString)) );
}


void CDDB::query( const QString &querystring )
{
   mMutex.lock();
   mRequestIsQuery = true;
   mpHits->clear();
   mpCount->setText( tr("q:" ) );
   genericrequest( querystring );
}


void CDDB::read( const QString &querystring )
{
   if( querystring.isEmpty() )
   {
      return;
   }
   mMutex.lock();
   QString readheader( "cddb read " );
   readheader.append( querystring );
   QStringList list( readheader.split(' ') );
   readheader = list.at(0)
        + ' ' + list.at(1)
        + ' ' + list.at(2)
        + ' ' + list.at(3);
   mRequestIsQuery = false;
   mpCount->setText( tr("r:" ) );
   genericrequest( readheader );
}


void CDDB::httpRequestFinished( int requestId, bool /*error*/ )
{
   int i;
   
   if ( requestId != mHttpGetId )
   {
      return;
   }
   
   mpFile->close();
   mpFile->open( QIODevice::ReadOnly );
   QString data( QString::fromUtf8( mpFile->readAll() ) );
   mpFile->close();
   mpFile->remove();
   mMutex.unlock();
   
   data.remove( '\r' );
   data.replace( '"', "''" );
   QStringList response( data.split('\n') );
   
#if 0
   for( i = 0; i < response.size(); i++ )
   {
TRACEMSG << response.at(i);
   }
#endif
   
   if( mRequestIsQuery )
   {
      QStringList status( response.at(0).split(' ') );
      
      switch( status.at(0).toULong() )
      {
         case 200:
            /* got one */
            mpHits->addItem( response.at(0).mid(4) );
            break;
         case 202:
            /* found none */
            for( i = 0; i < 100; i++ )
            {
               mpToc->mTitle[i].clear();
            }
            emit tocUpdated();
            break;
         case 210:
         case 211:
            /* got many */
            for( i = 1; i < response.size(); i++ )
            {
               if( response.at(i).size() > 1 )
               {
                  mpHits->addItem( response.at(i) );
               }
            }
            break;
         default:
            break;
      }
      
      mpCount->setText( QString::number( mpHits->count() ) + ":" );
   }
   else
   {
      for( i = 0; i < 100; i++ )
      {
         mpToc->mTitle[i].clear();
      }
      
      for( i = 1; i < response.size(); i++ )
      {
         QString line( response.at(i) );
         
         if( line.startsWith( "DTITLE" ) )
         {
            mpToc->mTitle[0] += line.mid( line.indexOf('=')+1 );
         }
         
         if( line.startsWith( "DYEAR" ) )
         {
            mpToc->mYear = line.mid( line.indexOf('=')+1 ).toLong();
         }
         
         if( line.startsWith( "DGENRE" ) )
         {
            mpToc->mGenre = line.mid( line.indexOf('=')+1 );
         }
         
         if( line.startsWith( "TTITLE" ) )
         {
            long tracknr = line.mid( 6, line.indexOf('=')-6 ).toLong();
            mpToc->mTitle[tracknr+1] += line.mid( line.indexOf('=')+1 );
         }
      }
      emit tocUpdated();
   }
}


void CDDB::genericrequest( const QString &cmd )
{
   if( mCancel ) return;
   QString request( "/~cddb/cddb.cgi?cmd=" );
   request.append( cmd );
   request.append( "&hello=svolli svolli.de Stripped prototype&proto=6" );
   request.replace( ' ', '+' );
   
   mpFile->open( QIODevice::WriteOnly );
   mHttpGetId = mpHttp->get( request, mpFile );
}


void CDDB::cancel()
{
   mCancel = true;
   mpHttp->abort();
}
