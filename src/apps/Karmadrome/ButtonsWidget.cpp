/**
 * src/apps/Karmadrome/ButtonsWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "ButtonsWidget.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"

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
   
   connect( mpSignalMapper, SIGNAL(mapped(QWidget*)),
            this, SIGNAL(clicked(QWidget*)) );
   
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
   
   connect( mpSignalMapper, SIGNAL(mapped(QWidget*)),
            this, SIGNAL(clicked(QWidget*)) );
   
   setLayout( mpMainLayout );
}


void ButtonsWidget::updateButtons( const QStringList &fileNames )
{
   int i;
   int rows = MySettings().VALUE_NUMBEROFCOLUMNS;
   
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
      pb->setObjectName( QString( "FolderButton" ) );
      pb->setCheckable( true );
      pb->setToolTip( fileNames.at(i) );
      mButtonList.append( pb );
      connect( pb, SIGNAL(clicked()), mpSignalMapper, SLOT(map()) );
      mpSignalMapper->setMapping( pb, (QWidget*)pb );
      mpMainLayout->addWidget( pb, i / rows, i % rows );
      mpMainLayout->setRowStretch( i / rows, 0 );
   }
   mpMainLayout->setRowStretch( (i / rows) + 1, 1 );
}


void ButtonsWidget::lockButtons( const QStringList &list )
{
   int i, n;
   
   for( n = 0; n < mButtonList.size(); n++ )
   {
      mButtonList.at(n)->setChecked( false );
      for( i = 0; i < list.size(); i++ )
      {
         if( list.at(i) == mButtonList.at(n)->text() )
         {
            mButtonList.at(n)->setChecked( true );
            break;
         }
      }
   }
}