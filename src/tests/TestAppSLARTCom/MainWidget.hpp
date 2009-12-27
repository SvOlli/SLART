/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>

class QIcon;
class QString;
class QListWidget;
class QLineEdit;

class SLARTCom;

class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   void handleInput();
   void addDebug( const QString &message );
   void addMessage( const QByteArray &message, QListWidget *list = 0 );
   
signals:
   void sendText( const QByteArray &text );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   SLARTCom     *mpSLARTCom;
   QListWidget  *mpDebugBuffer;
   QListWidget  *mpMessageBuffer;
   QLineEdit    *mpInput;
};

#endif
