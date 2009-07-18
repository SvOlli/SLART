/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* define the features of main window to be used */
#define MAINWINDOW_SORCERER      0
#define MAINWINDOW_CHANGETITLE   0
#define MAINWINDOW_PROHIBITCLOSE 0

#include <QWidget>

class QIcon;
class QString;
class QHttpRequestHeader;
class QHttpResponseHeader;
class QTcpSocket;

class WebServer;

class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   void request( QTcpSocket *id, 
                 const QHttpRequestHeader &header );
   
signals:
   void response( QTcpSocket *id,
                  const QHttpResponseHeader &header,
                  const QByteArray &data );
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   WebServer *mpWebServer;
};

#endif
