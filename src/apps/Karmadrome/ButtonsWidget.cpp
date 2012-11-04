/*
 * src/apps/Karmadrome/ButtonsWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ButtonsWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */
#include "KarmadromeConfigDialog.hpp"
#include "Settings.hpp"


ButtonsWidget::ButtonsWidget( QWidget *parent )
: QGroupBox( parent )
, mpMainLayout( new QGridLayout( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mButtonList()
{
   mpMainLayout->setContentsMargins( 3, 3, 3, 3 );

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
   mpMainLayout->setContentsMargins( 3, 3, 3, 3 );

   connect( mpSignalMapper, SIGNAL(mapped(QWidget*)),
            this, SIGNAL(clicked(QWidget*)) );

   setLayout( mpMainLayout );
}


void ButtonsWidget::updateButtons( const QStringList &fileNames )
{
   int i = 0;
   int rows = Settings::value( Settings::KarmadromeNumberOfColumns );

   for( i = 0; i < mButtonList.count(); i++ )
   {
      mpMainLayout->removeWidget( mButtonList.at(i) );
      delete mButtonList.at(i);
   }
   mButtonList.clear();

   QAbstractButton *button = 0;
   for( i = 0; i < fileNames.count(); i++ )
   {
      int lastSlash = fileNames.at(i).lastIndexOf( '/' );
      int lastDot   = fileNames.at(i).lastIndexOf( '.' );
      QString label( fileNames.at(i).mid( lastSlash+1, lastDot-lastSlash-1 ) );
      if( Settings::value( Settings::KarmadromeUseCheckBoxes ) )
      {
         button = new QCheckBox( label, this );
      }
      else
      {
         button = new QPushButton( label, this );
      }
      button->setObjectName( QString( "FolderButton" ) );
      button->setCheckable( true );
      button->setToolTip( fileNames.at(i) );
      mButtonList.append( button );
      connect( button, SIGNAL(clicked()),
               mpSignalMapper, SLOT(map()) );
      mpSignalMapper->setMapping( button, static_cast<QWidget*>(button) );
      mpMainLayout->addWidget( button, i / rows, i % rows );
      mpMainLayout->setRowStretch( i / rows, 0 );
   }
   mpMainLayout->setRowStretch( (i / rows) + 1, 1 );
}


void ButtonsWidget::lockButtons( const QStringList &list )
{
   foreach( QAbstractButton *button, mButtonList )
   {
      button->setChecked( false );
      foreach( const QString &entry, list )
      {
         if( entry == button->text() )
         {
            button->setChecked( true );
            break;
         }
      }
   }
}
