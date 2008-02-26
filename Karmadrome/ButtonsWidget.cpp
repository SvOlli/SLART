/**
 * ButtonsWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ButtonsWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>


ButtonsWidget::ButtonsWidget( QWidget *parent )
: QGroupBox( parent )
, mpMainLayout( new QGridLayout( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mButtonList()
{
#if QT_VERSION < 0x040300
   mpMainLayout->setMargin( 3 );
#else
   mpMainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   connect(mpSignalMapper, SIGNAL(mapped(const QString &)),
           this, SIGNAL(clicked(const QString &)));
   
   setLayout( mpMainLayout );
}


ButtonsWidget::ButtonsWidget( const QString &title, QWidget *parent )
: QGroupBox( title, parent )
, mpMainLayout( new QGridLayout( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mButtonList()
{
#if QT_VERSION < 0x040300
   mpMainLayout->setMargin( 3 );
#else
   mpMainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   connect(mpSignalMapper, SIGNAL(mapped(const QString &)),
           this, SIGNAL(clicked(const QString &)));
   
   setLayout( mpMainLayout );
}


void ButtonsWidget::updateButtons( const QStringList &fileNames )
{
   int i;
   int rows = MySettings().value( "ButtonRows", 3 ).toInt();
   
   for( i = 0; i < mButtonList.count(); i++ )
   {
      mpMainLayout->removeWidget( mButtonList.at(i) );
      delete mButtonList.at(i);
   }
   mButtonList.clear();
   
   for( i = 0; i < fileNames.count(); i++ )
   {
      int lastSlash = fileNames.at(i).lastIndexOf( '/' );
      int lastDot   = fileNames.at(i).lastIndexOf( '.' );
      QString label( fileNames.at(i).mid( lastSlash+1, lastDot-lastSlash-1 ) );
      QPushButton *pb = new QPushButton( label, this );
      mButtonList.append( pb );
      connect( pb, SIGNAL(clicked()), mpSignalMapper, SLOT(map()) );
      mpSignalMapper->setMapping( pb, fileNames.at(i) );
      mpMainLayout->addWidget( pb, i / rows, i % rows );
   }
}
