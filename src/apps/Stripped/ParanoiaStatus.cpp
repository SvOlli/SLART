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
#include <cdio/cdio.h>
#if CDIO_API_VERSION <= 5
#include <cdio/cdda.h>
#else
#include <cdio/paranoia/cdda.h>
#endif

/* Qt headers */
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QToolBar>

/* local library headers */

/* local headers */


static const unsigned int NUMBER_OF_TRACKS  = 100;
static const unsigned int NUMBER_OF_COLUMNS = sizeof(strerror_tr) / sizeof(const char*);

ParanoiaStatus::ParanoiaStatus( QWidget *parent )
: QWidget( parent )
, mpData( new int[NUMBER_OF_TRACKS * NUMBER_OF_COLUMNS] )
, mpLayout( new QGridLayout( this ) )
, mpError( new QLabel( tr("Error:" ) ) )
, mpTrackNr( new QSpinBox( this ) )
, mpTotal( new QLabel( tr("Total:" ) ) )
, mpHeaders( new QLabel*[NUMBER_OF_COLUMNS] )
, mpTrackErrors( new QLabel*[NUMBER_OF_COLUMNS] )
, mpTotalErrors( new QLabel*[NUMBER_OF_COLUMNS] )
, mpToolBar( 0 )
{
   mpTrackNr->setRange( 0, 99 );
   mpTrackNr->setValue( 1 );
   mpTrackNr->setAlignment( Qt::AlignRight );
   mpLayout->setContentsMargins( 0, 0, 0, 0 );
   mpLayout->setSpacing( 0 );
   for( unsigned int i = 0; i < NUMBER_OF_COLUMNS; i++ )
   {
      mpHeaders[i] = new QLabel( QString::number(i), this );
      mpHeaders[i]->setAlignment( Qt::AlignCenter );
      mpHeaders[i]->setMinimumWidth( QFontMetrics( QFont() ).horizontalAdvance( "00000 " ) );
      mpTrackErrors[i] = new QLabel( this );
      mpTrackErrors[i]->setObjectName( "TrackErrorsLabel" );
      mpTrackErrors[i]->setToolTip( strerror_tr[i] );
      mpTrackErrors[i]->setAlignment( Qt::AlignCenter );
      mpTotalErrors[i] = new QLabel( this );
      mpTotalErrors[i]->setObjectName( "TotalErrorsLabel" );
      mpTotalErrors[i]->setToolTip( strerror_tr[i] );
      mpTotalErrors[i]->setAlignment( Qt::AlignCenter );
   }
   changeOrientation( Qt::Horizontal );
   connect( mpTrackNr, SIGNAL(valueChanged(int)),
            this, SLOT(showTrackStats(int)) );
   clear();
}


ParanoiaStatus::~ParanoiaStatus()
{
   delete[] mpHeaders;
   delete[] mpTrackErrors;
   delete[] mpTotalErrors;
   delete[] mpData;
}


void ParanoiaStatus::setToolBar( QToolBar *toolBar )
{
   mpToolBar = toolBar;
}


void ParanoiaStatus::changeOrientation( Qt::Orientation orientation )
{
   mpLayout->addWidget( mpError, 0, 0 );
   if( orientation == Qt::Horizontal )
   {
      mpLayout->addWidget( mpTrackNr, 1, 0 );
      mpLayout->addWidget( mpTotal,   2, 0 );
      for( unsigned int i = 0; i < NUMBER_OF_COLUMNS; i++ )
      {
         mpLayout->addWidget( mpHeaders[i],     0, i + 1 );
         mpLayout->addWidget( mpTrackErrors[i], 1, i + 1 );
         mpLayout->addWidget( mpTotalErrors[i], 2, i + 1 );
      }
   }
   else
   {
      mpLayout->addWidget( mpTrackNr, 0, 1 );
      mpLayout->addWidget( mpTotal,   0, 2 );
      for( unsigned int i = 0; i < NUMBER_OF_COLUMNS; i++ )
      {
         mpLayout->addWidget( mpHeaders[i],     i + 1, 0 );
         mpLayout->addWidget( mpTrackErrors[i], i + 1, 1 );
         mpLayout->addWidget( mpTotalErrors[i], i + 1, 2 );
      }
   }
   adjustSize();
   if( mpToolBar )
   {
      mpToolBar->adjustSize();
   }
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


void ParanoiaStatus::setTrackData( int track, unsigned int elements, const unsigned long *counts )
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
