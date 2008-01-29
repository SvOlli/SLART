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


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /* prepare startup of application */
   void startUp();

public slots:
   /* handle call for next track from control widget */
   void getNextTrack( int player );
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );

signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   QWidget         *mpParent;
   ConfigDialog    *mpConfig;
   ControlWidget   *mpControl;
   PlaylistWidget  *mpPlaylist;
};

#endif
