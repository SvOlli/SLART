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

#include <QListWidget>

class QIcon;
class QString;


class MainWidget : public QListWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0 );
   
protected:
   /*  */
   virtual void keyPressEvent( QKeyEvent *event );
   /*  */
   virtual void keyReleaseEvent( QKeyEvent *event );
   
public slots:
   void msg( const QString &message );
   
signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
};

#endif
