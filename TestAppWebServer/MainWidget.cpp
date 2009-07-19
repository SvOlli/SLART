/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>

#include <QHttpRequestHeader>
#include <QHttpResponseHeader>
#include <QTcpSocket>

#include "WebServer.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpWebServer( new WebServer( this ) )
, mpMessageBuffer( new QListWidget( this ) )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
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
   
   mpMessageBuffer->setAlternatingRowColors( true );
   mpWebServer->start( 22222 );
   
}


void MainWidget::request( QTcpSocket *id, 
                          const QHttpRequestHeader &header )
{
   QString html("<h1>SvOlli was here...</h1>\n");
   html.append( header.method() );
   html.append( " " );
   html.append( header.path() );
   html.append( "\n<form method='post'>"
                "<input type='hidden' name='Hidden' value='HiddenTest'>"
                "<input type='submit' name='Submit' value='Test'>"
                "</form>" );
   
   emit response( id,
                  QHttpResponseHeader( 200, "OK" ),
                  html.toUtf8() );
   
   mpMessageBuffer->addItem( header.toString() );
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
