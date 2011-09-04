/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QIcon;
class QString;
class QListWidget;
class QLineEdit;

/* forward declaration of local classes */
class Satellite;


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
   MainWidget( const MainWidget &that );
   MainWidget &operator=( const MainWidget &that );

   Satellite    *mpSatellite;
   QListWidget  *mpDebugBuffer;
   QListWidget  *mpMessageBuffer;
   QLineEdit    *mpInput;
};

#endif
