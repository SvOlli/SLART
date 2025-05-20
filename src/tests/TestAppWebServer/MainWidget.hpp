/*
 * src/tests/TestAppWebServer/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QByteArray>
#include <QList>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QIcon;
class QListWidget;
class QString;
class QHttpRequestHeader;
class QHttpResponseHeader;
class QTcpSocket;

/* forward declaration of local classes */
class Database;
class Satellite;
class WebServer;


class MainWidget : public QWidget
{
   Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );
   virtual ~MainWidget();

public slots:
   void request( QTcpSocket *id,
                 const QHttpRequestHeader &header );
   void newMsg( const QByteArray &msg );

signals:
   void response( QTcpSocket *id,
                  const QHttpResponseHeader &header,
                  const QByteArray &data );

private:
   Q_DISABLE_COPY( MainWidget )

   Database             *mpDatabase;
   WebServer            *mpWebServer;
   QListWidget          *mpMessageBuffer;
   Satellite            *mpSatellite;
   QList<QTcpSocket*>   mDelayList;
   QByteArray           mMsg;
   TrackInfo            mTrackInfo;
};

#endif
