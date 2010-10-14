/**
 * src/libs/Common/WebServer.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "WebServer.hpp"

#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QTcpServer>
#include <QTcpSocket>

#if 0
#include "Trace.hpp"
#endif


WebServer::WebServer( QObject *parent )
: QObject( parent )
, mpParent( parent )
, mpTcpServer( 0 )
, mpTcpSocket( 0 )
, mWebDir()
{
}


WebServer::~WebServer()
{
   stop();
}


bool WebServer::start( quint16 port, const QString &webPath )
{
#if 0
TRACESTART(WebServer::start)
#endif
   bool success;
   mWebDir.setPath( webPath );
   mpTcpServer = new QTcpServer( this );
   success = mpTcpServer->listen( QHostAddress::Any, port );
   if( !success )
   {
#if 0
TRACEMSG << mpTcpServer->serverError() << mpTcpServer->errorString();
#endif
   }
   connect( mpTcpServer, SIGNAL(newConnection()),
            this, SLOT(handleNewConnection()) );
   return success;
}


void WebServer::stop()
{
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
   QString    line;
   QString    key;
   QString    value;
   ulong      content_length = 0;
   QByteArray data;
   
   if( !mpTcpServer )
   {
      return;
   }
   QTcpSocket *socket = mpTcpServer->nextPendingConnection();
   if( !socket )
   {
      return;
   }
   connect( socket, SIGNAL(disconnected()),
            socket, SLOT(deleteLater()) );
   socket->waitForReadyRead();
   line = QString::fromUtf8( socket->readLine() );
   QStringList requestLine( line.split( QString(" ") ) );
   if( requestLine.size() < 3 )
   {
      return;
   }
   
   if( mWebDir.entryList().contains( requestLine.at(1).mid(1) ) )
   {
      QFile file( mWebDir.path() + requestLine.at(1) );
      if( file.open( QIODevice::ReadOnly ) )
      {
         response( socket,
                   QHttpResponseHeader( 200, "OK" ),
                   file.readAll() );
         file.close();
      }
      else
      {
         int errcode;
         switch( file.error() )
         {
            case QFile::ReadError:
            case QFile::FatalError:
            case QFile::OpenError:
               errcode = 404;
               break;
            case QFile::PermissionsError:
               errcode = 403;
               break;
            default:
               errcode = 401;
               break;
         }
         response( socket,
                   QHttpResponseHeader( errcode, "Error" ),
                   QByteArray( tr("An error occured").toUtf8() ) );
      }
   }
   else
   {
      QHttpRequestHeader header( requestLine.at(0), requestLine.at(1) );
      int colpos;
      for(;;)
      {
         line = QString::fromUtf8(socket->readLine());
         line.remove( "\n" );
         line.remove( "\r" );
         if( line.isEmpty() )
         {
            break;
         }
         colpos = line.indexOf( ": " );
         key = line.left( colpos );
         value = line.mid( colpos + 2 );
         if( key == "Content-Length" )
         {
            content_length = value.toULong();
         }
         header.addValue( key, value );
      }
      data = socket->read( content_length );
      
      if( content_length > 0 )
      {
         QStringList form( QString::fromUtf8( data ).split('&') );
         for( int i = 0; i < form.size(); i++ )
         {
            colpos = form.at(i).indexOf( "=" );
            key = form.at(i).left( colpos );
            value = form.at(i).mid( colpos + 1 );
            key.prepend( "POST: " );
            header.addValue( key, value );
         }
      }
      
      emit request( socket, header );
   }
}


void WebServer::response( QTcpSocket *id,
                          const QHttpResponseHeader &header,
                          const QByteArray &data )
{
   QHttpResponseHeader responseHeader( header );
   responseHeader.addValue( "Size", QString::number( data.size() ) );
   id->write( responseHeader.toString().toUtf8() );
   id->write( data );
   id->waitForBytesWritten( -1 );
   id->disconnectFromHost();
}
