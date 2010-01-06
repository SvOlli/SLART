/**
 * EventHandler.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP EVENTHANDLER_HPP

#include <QMainWindow>

class QIcon;
class QString;
class QListWidget;


class EventHandler : public QMainWindow
{
Q_OBJECT
   
public:
   EventHandler( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~EventHandler();
   
public slots:
   /* handle shortcuts */
   void actionNext();
   void actionPause();
   void actionPower();

private:
   EventHandler( const EventHandler &other );
   EventHandler &operator=( const EventHandler &other );
};

#endif
