/**
 * ScrollLine.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SCROLLLINE_HPP
#define SCROLLLINE_HPP SCROLLLINE_HPP

#include <QLineEdit>

class QTimer;
class QString;

class ScrollLine : public QLineEdit
{
Q_OBJECT

public:
   ScrollLine( QWidget *parent );
   virtual ~ScrollLine();
   
public slots:
   /* callback for timer to scroll */
   void scrolling();
   /* intercept setText for resetting scroll position */
   void setText( const QString &text );
   
protected:
   virtual void focusInEvent( QFocusEvent *event );

signals:

private:
   ScrollLine( const ScrollLine &other );
   ScrollLine &operator=( const ScrollLine &other );

   static QTimer        *mpTimer;
   int                  mDirection;
   int                  mPosition;
};

#endif
