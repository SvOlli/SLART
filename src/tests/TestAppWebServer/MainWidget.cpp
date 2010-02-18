/**
 * src/tests/TestAppWebServer/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the Lesser GNU Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "MainWidget.hpp"

#include <QtGui>

#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QTcpSocket>

#include <Satellite.hpp>
#include <WebServer.hpp>
#include <MySettings.hpp>

MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpWebServer( new WebServer( this ) )
, mpMessageBuffer( new QListWidget( this ) )
, mpSatellite( new Satellite( this ) )
, mDelayList()
, mMsg()
{
   QBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
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
   MySettings().setValue( "UseSatellite", true );
   mpSatellite->restart();
   mpSatellite->send( "P0R" );
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
   if( header.path() == "/wait" )
   {
      mDelayList.append( id );
      return;
   }
   QString html("<html>\n<head>\n<title>SvOlli was here...</title>\n"
                "<meta http-equiv='content-type' content='text/html; charset=UTF-8'>\n"
                "<link rel='shortcut icon' href='/favicon.ico' type='image/x-icon'>\n"
                "<script src='ajax.js' type='text/javascript'>\n"
                "</script>\n"
                "</head>\n<body>\n"
         "<h1>SvOlli was here...</h1>\n");
   html.append( header.method() );
   html.append( " " );
   html.append( header.path() );
   html.append( "\n<h2>last Satellite message:</h2>\n<pre>" );
   html.append( QString::fromUtf8(mMsg) );
   html.append( "</pre><br>\n<form method='post'>\n"
                "<input type='hidden' name='Hidden' value='HiddenTest'>\n"
                "<input type='submit' name='Submit' value='Test'>\n"
                "</form>\n</body>\n</html>" );
   
   emit response( id,
                  QHttpResponseHeader( 200, "OK" ),
                  html.toUtf8() );
}


void MainWidget::newMsg( const QByteArray &msg )
{
   mMsg = msg;
   while( mDelayList.size() )
   {
      QTcpSocket *id = mDelayList.takeFirst();
      emit response( id,
                     QHttpResponseHeader( 200, "OK" ),
                     "reply" );
   }
}
