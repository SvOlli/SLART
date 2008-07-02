/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>


class ConfigDialog;
class ControlWidget;
class PlaylistWidget;
class QPushButton;

class Database;

class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   ~MainWidget();

   /* prepare startup of application */
   void startUp();

public slots:
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );
   /* don't autostart if database is empty */
   void allowAutostart( bool allow );

protected:
   /* handle resize event to place settings button */
   void resizeEvent( QResizeEvent *event );

signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   bool            mAllowAutostart;
   QWidget         *mpParent;
   Database        *mpDatabase;
   ConfigDialog    *mpConfig;
   PlaylistWidget  *mpPlaylist;
   ControlWidget   *mpControl;
   QPushButton     *mpSettingsButton;
};

#endif
