/**
 * MyClipboard.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MYCLIPBOARD_HPP
#define MYCLIPBOARD_HPP MYCLIPBOARD_HPP

#include <QWidget>

class QLabel;
class QComboBox;
class QString;

class MyClipboard : public QWidget
{
Q_OBJECT
   
public:
   MyClipboard( QWidget *parent = 0, bool withLayout = true );

   static void set( const QString &text );
   static QString get();
   void readSettings();
   void writeSettings();

   QLabel       *label()    { return mpLabel;    };
   QComboBox    *comboBox() { return mpComboBox; };

private:
   QLabel       *mpLabel;
   QComboBox    *mpComboBox;
};

#endif
