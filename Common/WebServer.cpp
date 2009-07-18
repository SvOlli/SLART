/**
 * WebServer.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "WebServer.hpp"

#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QTcpServer>
#include <QTcpSocket>

#include "Trace.hpp"


WebServer::WebServer( QObject *parent )
: QObject( parent )
, mpParent( parent )
, mpTcpServer( 0 )
, mpTcpSocket( 0 )
{
TRACESTART(WebServer::WebServer)
}


WebServer::~WebServer()
{
TRACESTART(WebServer::~WebServer)
   stop();
}


bool WebServer::start( quint16 port )
{
TRACESTART(WebServer::start)
   bool success;
   mpTcpServer = new QTcpServer( this );
   success = mpTcpServer->listen( QHostAddress::Any, port );
   if( !success )
   {
TRACEMSG << mpTcpServer->serverError() << mpTcpServer->errorString();
      
   }
   connect( mpTcpServer, SIGNAL(newConnection()),
            this, SLOT(handleNewConnection()) );
   return success;
}


void WebServer::stop()
{
TRACESTART(WebServer::stop)
   
   if( mpTcpServer )
   {
      disconnect( mpTcpServer, SIGNAL(newConnection()),
                  this, SLOT(handleNewConnection()) );
      delete mpTcpServer;
      mpTcpServer = 0;
   }
   
   if( mpTcpSocket )
   {
      delete mpTcpSocket;
      mpTcpSocket = 0;
   }
}


void WebServer::handleNewConnection()
{
TRACESTART(Webserver::handleNewConnection)
   QString line;
   QString key;
   QString value;
   if( !mpTcpServer )
   {
      return;
   }
   QTcpSocket *socket = mpTcpServer->nextPendingConnection();
   if( !socket )
   {
      return;
   }
   socket->waitForReadyRead();
   line = QString::fromUtf8( socket->readLine() );
TRACEMSG << line;
   QStringList requestLine( line.split( QString(" ") ) );
TRACEMSG << requestLine;
   if( requestLine.size() < 3 )
   {
      return;
   }
   QHttpRequestHeader header( requestLine.at(0), requestLine.at(1) );
   int colpos;
   for(;;)
   {
      line = QString::fromUtf8(socket->readLine());
      line.remove( "\n" );
      line.remove( "\r" );
TRACEMSG << line;
      if( line.isEmpty() )
      {
         break;
      }
      colpos = line.indexOf( ": " );
      key = line.left( colpos );
      value = line.mid( colpos + 2 );
      header.addValue( key, value );
   }
   emit request( socket, header );
TRACEMSG << header.toString();
}


void WebServer::response( QTcpSocket *id,
                          const QHttpResponseHeader &header,
                          const QByteArray &data )
{
TRACESTART(WebServer::response)
   QHttpResponseHeader responseHeader( header );
   responseHeader.addValue( "Size", QString::number( data.size() ) );
   id->write( responseHeader.toString().toUtf8() );
   id->write( data );
   id->close();
}
