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
class QMouseEvent;

class ScrollLine : public QLineEdit
{
Q_OBJECT

public:
   ScrollLine( QWidget *parent, bool autoScroll = true );
   virtual ~ScrollLine();

public slots:
   /* callback for timer to scroll */
   void scrolling();
   /* intercept setText for resetting scroll position */
   void setText( const QString &text );
   
protected:
   /* handle mouse event for correct copy'n'paste */
   void mouseDoubleClickEvent( QMouseEvent *event );
   /* handle mouse event for correct copy'n'paste */
   void mousePressEvent( QMouseEvent *event );
   /* handle mouse event for correct copy'n'paste */
   void mouseReleaseEvent( QMouseEvent *event );

signals:

private:
   ScrollLine( const ScrollLine &other );
   ScrollLine &operator=( const ScrollLine &other );

   static QTimer        *mpTimer;
   bool                 mClicked;
   int                  mDirection;
   int                  mPosition;
};

#endif
