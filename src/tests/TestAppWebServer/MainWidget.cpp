/**
 * src/tests/TestAppWebServer/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the Lesser GNU Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QTcpSocket>

/* local library headers */
#include <Database.hpp>
#include <Satellite.hpp>
#include <WebServer.hpp>
#include <MySettings.hpp>

/* local headers */


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpWebServer( new WebServer( this ) )
, mpMessageBuffer( new QListWidget( this ) )
, mpSatellite( new Satellite( this ) )
, mDelayList()
, mMsg("-")
, mTrackInfo()
{
   QBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   mpSatellite->setTestApp( true );
   mpSatellite->restart();
   
   mainLayout->addWidget( mpMessageBuffer );
   setLayout( mainLayout );
   
   connect( mpWebServer, SIGNAL(request( QTcpSocket *, const QHttpRequestHeader & )),
            this, SLOT(request( QTcpSocket *, const QHttpRequestHeader & )) );
   connect( this, SIGNAL(response( QTcpSocket *, const QHttpResponseHeader &, const QByteArray & )),
            mpWebServer, SLOT(response( QTcpSocket *, const QHttpResponseHeader &, const QByteArray & )) );
   connect( mpSatellite, SIGNAL(received(const QByteArray &) ),
            this, SLOT(newMsg(const QByteArray &)) );
   
   mpMessageBuffer->setAlternatingRowColors( true );
   mpWebServer->start( 22222, QString(":/web/") );

   mTrackInfo.mArtist = "-";
   mTrackInfo.mTitle = "-";
   mTrackInfo.mAlbum = "-";

   mpSatellite->waitForConnected();
   mpSatellite->send( "P0R" );
}


MainWidget::~MainWidget()
{
   if( mpDatabase )
   {
      delete mpDatabase;
      mpDatabase = 0;
   }
}


void MainWidget::request( QTcpSocket *id, 
                          const QHttpRequestHeader &header )
{
   mpMessageBuffer->addItem( header.toString() );
   while( mpMessageBuffer->count() > 200 )
   {
      QListWidgetItem *item = mpMessageBuffer->takeItem( 0 );
      if( item )
      {
         delete item;
      }
   }
   mpMessageBuffer->scrollToBottom();

#if 0
   if( header.path() == "/favicon.ico" )
   {
      QFile file( ":/favicon.ico" );
      file.open( QIODevice::ReadOnly | QIODevice::Text );
      emit response( id,
                     QHttpResponseHeader( 200, "OK" ),
                     file.readAll() );
      file.close();
      return;
   }
#endif
   if( header.path() == "/wait" )
   {
      mDelayList.append( id );
      return;
   }
   if( header.path() == "/get" )
   {
      QString reply( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
                     "<response>\n"
                     " <reply>%1</reply>\n"
                     " <artist>%2</artist>\n"
                     " <title>%3</title>\n"
                     " <album>%4</album>\n"
                     " <track>%5</track>\n"
                     "</response>\n" );
      QByteArray replyMsg( reply.arg( Qt::escape( QString::fromUtf8(mMsg) ),
                                      Qt::escape( mTrackInfo.mArtist ),
                                      Qt::escape( mTrackInfo.mTitle ),
                                      Qt::escape( mTrackInfo.mAlbum ),
                                      QString::number(mTrackInfo.mTrackNr) ).toUtf8() );
      emit response( id,
                     QHttpResponseHeader( 200, "OK XML" ),
                     replyMsg );
      return;
   }
   QString html("<html>\n<head>\n<title>%1</title>\n"
         "<meta http-equiv='content-type' content='text/html; charset=UTF-8'>\n"
         "<link rel='shortcut icon' href='/favicon.ico' type='image/x-icon'>\n"
         "<script src='ajax.js' type='text/javascript'>\n"
         "</script>\n"
         "</head>\n<body>\n"
         "<h1>%2</h1>\n"
         "<p>HTTP Request: %3 %4</p>\n"
         "<table border='1'>\n"
         "<tr valign='top'><td>Artist:</td><td><textarea cols='80' rows='1' name='artist' wrap='off' readonly></textarea></td></tr>\n"
         "<tr valign='top'><td>Title:</td><td><textarea cols='80' rows='1' name='title' wrap='off' readonly></textarea></td></tr>\n"
         "<tr valign='top'><td>Album:</td><td><textarea cols='80' rows='1' name='album' wrap='off' readonly></textarea></td></tr>\n"
         "<tr valign='top'><td>Track:</td><td><textarea cols='80' rows='1' name='track' wrap='off' readonly></textarea></td></tr>\n"
         "<tr valign='top'><td>Reply:</td><td><textarea cols='80' rows='5' name='reply' wrap='off' readonly></textarea></td></tr>\n"
         "<tr valign='top'><td>Status:</td><td><input name='status' size='12' readonly>"
         "<input type='button' value='refresh' onclick='loadFields();'></td></tr>\n"
         "</table></body></html>\n"
         );
   html = html.arg( QApplication::organizationName() + ": " + QApplication::applicationName(),
                    QApplication::organizationName() + ": " + QApplication::applicationName(),
                    header.method(),
                    header.path() );
   
   emit response( id,
                  QHttpResponseHeader( 200, "OK" ),
                  html.toUtf8() );
}


void MainWidget::newMsg( const QByteArray &msg )
{
   bool gotData( false );

   mMsg = msg;

   QStringList list( QString::fromUtf8(mMsg).split('\n') );
   if( (list.size() > 1) && (list.at(0) == "p0p") )
   {
      gotData = mpDatabase->getTrackInfo( 0, list.at(1) );
   }
   if( (list.size() > 0) && (list.at(0) == "p0s") )
   {
      mTrackInfo.clear();
      mTrackInfo.mArtist = tr("-");
      mTrackInfo.mTitle = tr("-");
      mTrackInfo.mAlbum = tr("-");
   }
   if( gotData )
   {
      mpDatabase->getTrackInfo( &mTrackInfo, list.at(1) );
   }

   while( mDelayList.size() )
   {
      QTcpSocket *id = mDelayList.takeFirst();
      emit response( id,
                     QHttpResponseHeader( 200, "OK WAIT" ),
                     "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><response><done>1</done></response>" );
   }
}
