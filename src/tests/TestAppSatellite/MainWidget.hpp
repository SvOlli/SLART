/*
 * src/tests/TestAppSatellite/MainWidget.hpp
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
   void addDebug( const QByteArray &message );
   void addMessage( const QByteArray &message, QListWidget *list = 0 );

signals:
   void sendText( const QByteArray &text );

private:
   Q_DISABLE_COPY( MainWidget )

   Satellite    *mpSatellite;
   QListWidget  *mpDebugBuffer;
   QListWidget  *mpMessageBuffer;
   QLineEdit    *mpInput;
};

#endif
