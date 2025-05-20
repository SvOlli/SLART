/*
 * \file src/tests/TestAppWebServer/MainWidget.hpp
 * \author written by Sven Oliver Moll
 *
 * \copyright
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
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );
   virtual ~MainWidget();

public slots:

signals:

private:
   Q_DISABLE_COPY( MainWidget )
};

#endif
