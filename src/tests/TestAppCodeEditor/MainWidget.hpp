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

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QListWidget;
class QPushButton;

/* forward declaration of local classes */
class CodeEditor;
class LuaEditorDialog;
class MyLua;


class MainWidget : public QWidget
{
   Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();

public slots:
   void log( const QString &msg, bool error = false );

signals:
   void runCode( const QString &code );

private:
   Q_DISABLE_COPY( MainWidget )

   MyLua                *mpLua;
   QPushButton          *mpOpenEditorButton;
   QListWidget          *mpMessageBuffer;
   bool                 mMessageBufferAlternateColor;
};

#endif
