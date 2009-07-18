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
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   setLayout( mainLayout );
   
   connect( mpWebServer, SIGNAL(request( QTcpSocket *, const QHttpRequestHeader & )),
            this, SLOT(request( QTcpSocket *, const QHttpRequestHeader & )) );
   connect( this, SIGNAL(response( QTcpSocket *, const QHttpResponseHeader &, const QByteArray & )),
            mpWebServer, SLOT(response( QTcpSocket *, const QHttpResponseHeader &, const QByteArray & )) );
   
   mpWebServer->start( 22222 );
   
}


void MainWidget::request( QTcpSocket *id, 
                          const QHttpRequestHeader &header )
{
   emit response( id,
                  QHttpResponseHeader( 200, "OK" ),
                  QByteArray("<h1>SvOlli was here...</h1>")+header.path().toUtf8() );
}
