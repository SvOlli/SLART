/**
 * src/tests/TestAppWebServer/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>

#include <QByteArray>
#include <QList>

class QIcon;
class QListWidget;
class QString;
class QHttpRequestHeader;
class QHttpResponseHeader;
class QTcpSocket;

class Satellite;
class WebServer;

class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   void request( QTcpSocket *id, 
                 const QHttpRequestHeader &header );
   void newMsg( const QByteArray &msg );
   
signals:
   void response( QTcpSocket *id,
                  const QHttpResponseHeader &header,
                  const QByteArray &data );
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   WebServer   *mpWebServer;
   QListWidget *mpMessageBuffer;
   Satellite   *mpSatellite;

   QList<QTcpSocket*> mDelayList;
   QByteArray  mMsg;
};

#endif
