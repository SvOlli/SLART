/**
 * src/apps/Notorious/SearchWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SearchWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */

/* local headers */


SearchWidget::SearchWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mStar( '*' )
, mPercent( '%' )
, mQuote( '\'' )
, mDoubleQuote( "''" )
, mpTitle( new QLineEdit( this ) )
, mpExact( new QCheckBox( tr("Exact Match"), this ) )
, mpTrack( new QComboBox( this ) )
, mpMinTime( new QLineEdit( this ) )
, mpMaxTime( new QLineEdit( this ) )
, mpSearchButton( new QPushButton( tr("Search"), this ) )
//, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpSQLClause( new QLabel( this ) )
, mpQuery1Info( new QLabel( tr("search 1"), this ) )
, mpQuery2Info( new QLabel( tr("search 2"), this ) )
, mTime1()
, mTime2()
, mSQL()
{
   QGridLayout *layout = new QGridLayout( this );
   
   mpTrack->addItem( tr("Album Titles And Tracks") );
   mpTrack->addItem( tr("Album Titles Only") );
   mpTrack->addItem( tr("Tracks Only") );
   mpQuery1Info->setAlignment( Qt::AlignCenter );
   mpQuery2Info->setAlignment( Qt::AlignCenter );
   
   layout->setContentsMargins( 0, 0, 0, 0 );
   
   layout->addWidget( new QLabel( tr("Title") ), 0, 0 );
   layout->addWidget( mpTitle, 0, 1, 1, 5 );
   layout->addWidget( mpExact, 0, 6 );
   layout->addWidget( new QLabel( tr("Track Nr") ), 1, 0 );
   layout->addWidget( mpTrack, 1, 1, 1, 4 );
   layout->addWidget( mpQuery1Info, 1, 5 );
   layout->addWidget( mpQuery2Info, 1, 6 );
   layout->addWidget( new QLabel( tr("Time between") ), 2, 0 );
   layout->addWidget( mpMinTime, 2, 1 );
   layout->addWidget( new QLabel( tr("and") ), 2, 2 );
   layout->addWidget( mpMaxTime, 2, 3 );
   layout->addWidget( mpSearchButton, 2, 5, 1, 2 );
//   layout->addWidget( mpSettingsButton, 2, 6 );
   layout->addWidget( mpSQLClause, 3, 0, 1, 6 );
   
   layout->setColumnStretch( 4, 1 );
   
   setLayout( layout );
   
   connect( mpTitle, SIGNAL(textChanged(const QString&)),
            this, SLOT(generateSQL()) );
   connect( mpExact, SIGNAL(clicked()),
            this, SLOT(generateSQL()) );
   connect( mpTrack, SIGNAL(currentIndexChanged(int)),
            this, SLOT(generateSQL()) );
   connect( mpMinTime, SIGNAL(textChanged(const QString&)),
            this, SLOT(generateSQL()) );
   connect( mpMaxTime, SIGNAL(textChanged(const QString&)),
            this, SLOT(generateSQL()) );
   connect( mpTitle, SIGNAL(returnPressed()),
            this, SLOT(checkSubmit()) );
   connect( mpMinTime, SIGNAL(returnPressed()),
            this, SLOT(checkSubmit()) );
   connect( mpMaxTime, SIGNAL(returnPressed()),
            this, SLOT(checkSubmit()) );
   connect( mpSearchButton, SIGNAL(clicked()),
            this, SLOT(checkSubmit()) );
            
   generateSQL();
}


void SearchWidget::generateSQL()
{
   QString title( mpTitle->text() );
   QString track;
   switch( mpTrack->currentIndex() )
   {
      case 1:
         track = "= 0";
         break;
      case 2:
         track = "> 0";
         break;
      default:
         break;
   }
   QString mintime( mpMinTime->text() );
   QString maxtime( mpMaxTime->text() );
   title.replace( mQuote, mDoubleQuote );
   title.replace( mStar, mPercent );
   
   mpSearchButton->setDisabled( title.isEmpty() );
   
   mSQL.clear();
   
   if( !(mpTitle->text().isEmpty()) )
   {
      mSQL.append( "title like '" );
      if( !(mpExact->isChecked()) )
      {
         mSQL.append( "%" );
      }
      mSQL.append( title );
      if( !(mpExact->isChecked()) )
      {
         mSQL.append( "%" );
      }
      mSQL.append( "'" );
   }
   if( !(track.isEmpty()) )
   {
      mSQL.append( " and track " );
      mSQL.append( track );
   }
   
   if( !(mintime.isEmpty()) )
   {
      int colon = mintime.indexOf(":");
      int mins = 0;
      int secs = 0;
      int frames = 0;
      if( colon >= 0 )
      {
         mins = mintime.left( colon ).toInt();
         secs = mintime.mid( colon + 1 ).toInt();
      }
      else
      {
         secs = mintime.toInt();
      }
      frames = (mins * 60 + secs) * 75;
      if( frames > 0 )
      {
         mSQL.append( " and playtime >= " );
         mSQL.append( QString::number( frames ) );
      }
   }
   
   if( !(maxtime.isEmpty()) )
   {
      int colon = maxtime.indexOf(":");
      int mins = 0;
      int secs = 0;
      int frames = 0;
      if( colon >= 0 )
      {
         mins = maxtime.left( colon ).toInt();
         secs = maxtime.mid( colon + 1 ).toInt();
      }
      else
      {
         secs = maxtime.toInt();
      }
      frames = (mins * 60 + secs) * 75;
      if( frames > 0 )
      {
         mSQL.append( " and playtime <= " );
         mSQL.append( QString::number( frames ) );
      }
   }
   
   mSQL.append( ";" );
   
   mpSQLClause->setText( mSQL );
}


void SearchWidget::checkSubmit()
{
   if( mpSearchButton->isEnabled() )
   {
      emit submit( mSQL );
      mpTitle->setFocus();
   }
}


void SearchWidget::query1Running( bool r )
{
   if( r )
   {
      mTime1.start();
      mpQuery1Info->setText( "running" );
   }
   else
   {
      mpQuery1Info->setText( QString("ok:%1s").arg( (float)mTime1.elapsed()/1000 ) );
   }
}


void SearchWidget::query2Running( bool r )
{
   if( r )
   {
      mTime2.start();
      mpQuery2Info->setText( "running" );
   }
   else
   {
      mpQuery2Info->setText( QString("ok:%1s").arg( (float)mTime2.elapsed()/1000 ) );
   }
}
