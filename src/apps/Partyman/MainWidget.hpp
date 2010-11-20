/**
 * src/apps/Partyman/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
class QPushButton;

/* forward declaration of local classes */
class ConfigDialog;
class ControlWidget;
class Database;
class PlaylistControlWidget;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();

   /* prepare startup of application */
   void startUp();

protected:

public slots:
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );
   /* don't autostart if database is empty */
   void allowAutostart( bool allow );

signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &that );
   MainWidget &operator=( const MainWidget &that );

   bool                    mAllowAutostart;
   QWidget                 *mpParent;
   Database                *mpDatabase;
   ConfigDialog            *mpConfig;
   PlaylistControlWidget   *mpPlaylist;
   ControlWidget           *mpControl;
};

#endif
