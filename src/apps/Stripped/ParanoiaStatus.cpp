/*
 * src/apps/Stripped/ParanoiaStatus.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ParanoiaStatus.hpp"

/* system headers */
#define NEED_STRERROR_TR
#include <cdio/cdda.h>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>

/* local headers */


static const unsigned int NUMBER_OF_TRACKS  = 100;
static const unsigned int NUMBER_OF_COLUMNS = sizeof(strerror_tr) / sizeof(const char*);

ParanoiaStatus::ParanoiaStatus( QWidget *parent )
: QWidget( parent )
, mpData( new int[NUMBER_OF_TRACKS * NUMBER_OF_COLUMNS] )
, mpTrackNr( new QSpinBox( this ) )
, mpTrackErrors( new QLabel*[NUMBER_OF_COLUMNS] )
, mpTotalErrors( new QLabel*[NUMBER_OF_COLUMNS] )
{
   mpTrackNr->setRange( 0, 99 );
   mpTrackNr->setValue( 1 );
   mpTrackNr->setAlignment( Qt::AlignRight );
   QGridLayout *layout = new QGridLayout( this );
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->setSpacing( 0 );
   layout->addWidget( new QLabel( tr("Error:"), this ), 0, 0 );
   layout->addWidget( mpTrackNr, 1, 0 );
   layout->addWidget( new QLabel( tr("Total:"), this ), 2, 0 );
   for( unsigned int i = 0; i < NUMBER_OF_COLUMNS; i++ )
   {
      QLabel *header = new QLabel( QString::number(i), this );
      header->setAlignment( Qt::AlignCenter );
      header->setMinimumWidth( QFontMetrics( QFont() ).width( "000000 " ) );
      header->setToolTip( strerror_tr[i] );
      layout->addWidget( header,          0, i + 1 );
      mpTrackErrors[i] = new QLabel( this );
      mpTrackErrors[i]->setObjectName( "TrackErrorsLabel" );
      mpTrackErrors[i]->setToolTip( strerror_tr[i] );
      mpTrackErrors[i]->setAlignment( Qt::AlignCenter );
      layout->addWidget( mpTrackErrors[i], 1, i + 1 );
      mpTotalErrors[i] = new QLabel( this );
      mpTotalErrors[i]->setObjectName( "TotalErrorsLabel" );
      mpTotalErrors[i]->setToolTip( strerror_tr[i] );
      mpTotalErrors[i]->setAlignment( Qt::AlignCenter );
      layout->addWidget( mpTotalErrors[i], 2, i + 1 );
   }
   connect( mpTrackNr, SIGNAL(valueChanged(int)),
            this, SLOT(showTrackStats(int)) );
   clear();
}


ParanoiaStatus::~ParanoiaStatus()
{
   delete[] mpData;
}


bool ParanoiaStatus::hasErrors()
{
   for( unsigned int i = 0; i < NUMBER_OF_TRACKS * NUMBER_OF_COLUMNS; i++ )
   {
      switch( i % NUMBER_OF_COLUMNS )
      {
      case 0:
      case 1:
      case 9:
         break;
      default:
         if( mpData[i] )
         {
            return true;
         }
         break;
      }
   }
   return false;
}


void ParanoiaStatus::clear()
{
   mpTrackNr->setMaximum( 1 );
   for( unsigned int i = 0; i < NUMBER_OF_TRACKS * NUMBER_OF_COLUMNS; i++ )
   {
      mpData[i] = 0;
   }

   for( unsigned int i = 0; i < NUMBER_OF_COLUMNS; i++ )
   {
      mpTotalErrors[i]->setBackgroundRole( QPalette::NoRole);
      mpTotalErrors[i]->setNum( 0 );
   }

   if( mpTrackNr->value() == 1 )
   {
      showTrackStats( 1 );
   }
   else
   {
      mpTrackNr->setValue( 1 );
   }
}


void ParanoiaStatus::showTrackStats( int track )
{
   for( unsigned int i = 0; i < NUMBER_OF_COLUMNS; i++ )
   {
      int value = mpData[track * NUMBER_OF_COLUMNS + i];
      if( (i > 1) && (i != 9) && (value > 0) )
      {
         mpTrackErrors[i]->setText( QString("<b>%1</b>").arg( value ) );
      }
      else
      {
         mpTrackErrors[i]->setNum( value );
      }
   }
}


void ParanoiaStatus::update( int track, unsigned int elements, const unsigned long *counts )
{
   if( elements > NUMBER_OF_COLUMNS )
   {
      elements = NUMBER_OF_COLUMNS;
   }

   for( unsigned int i = 0; i < elements; i++ )
   {
      int sum = 0;
      mpData[track * NUMBER_OF_COLUMNS + i] = counts[i];
      for( unsigned int n = 0; n < NUMBER_OF_TRACKS; n++ )
      {
         sum += mpData[n * NUMBER_OF_COLUMNS + i];
      }
      if( (i > 1) && (i != 9) && (sum > 0) )
      {
         mpTotalErrors[i]->setText( QString("<b>%1</b>").arg( sum ) );
      }
      else
      {
         mpTotalErrors[i]->setNum( sum );
      }
   }

   if( mpTrackNr->value() == track )
   {
      showTrackStats( track );
   }
   else
   {
      if( track > mpTrackNr->maximum() )
      {
         mpTrackNr->setMaximum( track );
      }
      mpTrackNr->setValue( track );
   }
}
