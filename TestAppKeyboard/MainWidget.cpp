/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>


MainWidget::MainWidget( QWidget *parent )
: QListWidget( parent )
{
   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   setLayout( mainLayout );
   setFocusPolicy( Qt::WheelFocus );
   
   setMinimumSize( 640, 480 );
}


void MainWidget::msg( const QString &message )
{
   addItem( message );
   if( count() > 50 )
   {
      takeItem(0);
   }
   if( count() >= 0 )
   {
      scrollToItem( item(count() - 1) );
   }
}

void MainWidget::keyPressEvent( QKeyEvent *event )
{
   QString m( "MainWidget::keyPressEvent:" );
   m.append( " key:" );
   m.append( QString::number( event->key(), 16 ) );
   m.append( " text:\"" );
   m.append( event->text() );
   m.append( "\" nativeModifiers:" );
   m.append( QString::number( event->nativeModifiers(), 16 ) );
   m.append( " nativeScanCode:" );
   m.append( QString::number( event->nativeScanCode(), 16 ) );
   m.append( " nativeVirtualKey:" );
   m.append( QString::number( event->nativeVirtualKey(), 16 ) );
   msg( m );
   event->ignore();
}

void MainWidget::keyReleaseEvent( QKeyEvent *event )
{
   QString m( "MainWidget::keyReleaseEvent:" );
   m.append( " key:" );
   m.append( QString::number( event->key(), 16 ) );
   m.append( " text:\"" );
   m.append( event->text() );
   m.append( "\" nativeModifiers:" );
   m.append( QString::number( event->nativeModifiers(), 16 ) );
   m.append( " nativeScanCode:" );
   m.append( QString::number( event->nativeScanCode(), 16 ) );
   m.append( " nativeVirtualKey:" );
   m.append( QString::number( event->nativeVirtualKey(), 16 ) );
   msg( m );
   event->ignore();
}
