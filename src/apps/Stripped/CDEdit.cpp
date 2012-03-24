/*
 * src/apps/Stripped/CDEdit.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "CDEdit.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <TagList.hpp>

/* local headers */
#include "CDDBClient.hpp"
#include "CDEditCheckBox.hpp"
#include "CDEditLineEdit.hpp"
#include "CDInfo.hpp"


CDEdit::CDEdit( CDInfo *info, CDDBClient *cddbClient, QWidget *parent )
: QWidget( parent )
, mpCDInfo( info )
, mpCDDBClient( cddbClient )
, mpScrollArea( new QScrollArea( this ) )
, mpScrollWidget( new QWidget( this ) )
, mpMainLayout( new QGridLayout( mpScrollWidget ) )
, mpLabelDiscArtist( new QLabel( tr("Artist") + ":", this ) )
, mpLabelDiscTitle( new QLabel( tr("Title") + ":", this ) )
, mpLabelDiscGenre( new QLabel( tr("Genre") + ":", this ) )
, mpDiscArtist( new CDEditLineEdit( this ) )
, mpDiscTitle( new CDEditLineEdit( this ) )
, mpDiscGenre( new CDEditLineEdit( this ) )
, mpLabelDiscID( new QLabel( tr("DiscID"), this ) )
, mpDiscID( new QLabel( this ) )
, mpDiscPlaytime( new QLabel( this ) )
, mpLabelTrackNr( new QLabel( tr("Nr"), this ) )
, mpLabelEnqueueTrack( new QLabel( tr("NQ"), this ) )
, mpLabelTrackArtist( new QLabel( tr("Artist"), this ) )
, mpLabelTrackTitle( new QLabel( tr("Title"), this ) )
, mpLabelTrackYear( new QLabel( tr("Year"), this ) )
, mpLabelTrackPlaytime( new QLabel( tr("Playtime"), this ) )
, mpTrackNr( 0 )
, mpEnqueueTrack( 0 )
, mpTrackArtist( 0 )
, mpTrackTitle( 0 )
, mpTrackYear( 0 )
, mpTrackPlaytime( 0 )
, mLastColumn( 3 )
{
   QVBoxLayout *outerLayout  = new QVBoxLayout( this );
   setFocusPolicy(Qt::StrongFocus);
   outerLayout->setContentsMargins( 0, 0, 0, 0 );
#if 0
   mpLabelTrackNr->setFlat( true );
   mpLabelTrackArtist->setFlat( true );
   mpLabelTrackYear->setFlat( true );
#endif
#if 0
   mpLabelTrackNr->setContentsMargins( 0, 0, 0, 0 );
   mpLabelTrackArtist->setContentsMargins( 0, 0, 0, 0 );
   mpLabelTrackYear->setContentsMargins( 0, 0, 0, 0 );
   mpLabelTrackTitle->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
   mpLabelTrackPlaytime->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
#endif
   mpLabelDiscID->setFrameShape( QFrame::Box );
   mpLabelTrackNr->setFrameShape( QFrame::Box );
   mpLabelEnqueueTrack->setFrameShape( QFrame::Box );
   mpLabelTrackArtist->setFrameShape( QFrame::Box );
   mpLabelTrackTitle->setFrameShape( QFrame::Box );
   mpLabelTrackYear->setFrameShape( QFrame::Box );
   mpLabelTrackPlaytime->setFrameShape( QFrame::Box );
   mpDiscPlaytime->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

   mpTrackNr       = new QCheckBox*[100];
   mpEnqueueTrack  = new QCheckBox*[100];
   mpTrackArtist   = new QLineEdit*[100];
   mpTrackTitle    = new QLineEdit*[100];
   mpTrackYear     = new QLineEdit*[100];
   mpTrackPlaytime = new QLabel*[100];

   mpMainLayout->setSpacing( 1 );
   mpMainLayout->setColumnStretch( 2, 1 );
   mpMainLayout->setColumnStretch( 3, 1 );
   mpLabelEnqueueTrack->setToolTip( tr("enqueue track in Partyman after ripping") );

   mpMainLayout->addWidget( mpLabelDiscArtist, 0, 0 );
   mpMainLayout->addWidget( mpDiscArtist,      0, 1, 1, 4 );
   mpMainLayout->addWidget( mpLabelDiscTitle,  1, 0 );
   mpMainLayout->addWidget( mpDiscTitle,       1, 1, 1, 4 );
   mpMainLayout->addWidget( mpLabelDiscGenre,  2, 0 );
   mpMainLayout->addWidget( mpDiscGenre,       2, 1, 1, 4 );
   mpMainLayout->addWidget( mpLabelDiscID,     0, 5 );
   mpMainLayout->addWidget( mpDiscID,          1, 5 );
   mpMainLayout->addWidget( mpDiscPlaytime,    2, 5 );

   mpMainLayout->addWidget( mpLabelTrackNr,       3, 0 );
   mpMainLayout->addWidget( mpLabelEnqueueTrack,  3, 1 );
   mpMainLayout->addWidget( mpLabelTrackArtist,   3, 2 );
   mpMainLayout->addWidget( mpLabelTrackTitle,    3, 3 );
   mpMainLayout->addWidget( mpLabelTrackYear,     3, 4 );
   mpMainLayout->addWidget( mpLabelTrackPlaytime, 3, 5 );

   for( int i = 0; i < 100; i++ )
   {
      if( i < 10 )
      {
         mpTrackNr[i]       = new CDEditCheckBox( QString("  ")+QString::number(i), this );
      }
      else
      {
         mpTrackNr[i]       = new CDEditCheckBox( QString::number(i), this );
      }
      mpEnqueueTrack[i]  = new CDEditCheckBox( this );
      mpTrackArtist[i]   = new CDEditLineEdit( this );
      mpTrackTitle[i]    = new CDEditLineEdit( this );
      mpTrackYear[i]     = new CDEditLineEdit( this );
      mpTrackPlaytime[i] = new QLabel( this );
      mpTrackPlaytime[i]->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      mpMainLayout->addWidget( mpTrackNr[i],       4+i, 0 );
      mpMainLayout->addWidget( mpEnqueueTrack[i],  4+i, 1 );
      mpMainLayout->addWidget( mpTrackArtist[i],   4+i, 2 );
      mpMainLayout->addWidget( mpTrackTitle[i],    4+i, 3 );
      mpMainLayout->addWidget( mpTrackYear[i],     4+i, 4 );
      mpMainLayout->addWidget( mpTrackPlaytime[i], 4+i, 5 );
      if(i > 0)
      {
         mpTrackNr[i]->setCheckState( Qt::Checked );
      }
      setTrackHidden( i, true );
   }
   mpMainLayout->setRowStretch( 104, 1 );

   mpScrollWidget->setLayout( mpMainLayout );
   mpScrollArea->setWidget( mpScrollWidget );
   mpScrollArea->setWidgetResizable( true );
   outerLayout->addWidget( mpScrollArea );

   clear();
   setLayout( outerLayout );
}


CDEdit::~CDEdit()
{
}


void CDEdit::clear()
{
   mpDiscArtist->clear();
   mpDiscTitle->clear();
   mpDiscGenre->clear();
   mpDiscID->setText( "00000000" );
   mpDiscPlaytime->clear();

   for( int i = 0; i < 100; i++ )
   {
      mpTrackArtist[i]->clear();
      mpTrackTitle[i]->clear();
      mpTrackYear[i]->clear();
      setTrackHidden( i, true );
   }

   emit containsData( false );
}


bool CDEdit::isEmpty()
{
   return mpTrackNr[1]->isHidden();
}


void CDEdit::toggleAllRipFlags()
{
   for( int i = 1; i < 100; i++ )
   {
      if( mpTrackNr[i]->isEnabled() )
      {
         if( mpTrackNr[i]->checkState() == Qt::Unchecked )
         {
            mpTrackNr[i]->setCheckState( Qt::Checked );
         }
         else
         {
            mpTrackNr[i]->setCheckState( Qt::Unchecked );
         }
      }
   }
}


void CDEdit::toggleAllEnqueueFlags()
{
   for( int i = 1; i < 100; i++ )
   {
      if( mpEnqueueTrack[i]->isEnabled() )
      {
         if( mpEnqueueTrack[i]->checkState() == Qt::Unchecked )
         {
            mpEnqueueTrack[i]->setCheckState( Qt::Checked );
         }
         else
         {
            mpEnqueueTrack[i]->setCheckState( Qt::Unchecked );
         }
      }
   }
}


void CDEdit::copyTrackArtist()
{
   QString artist( mpDiscArtist->text() );

   for( int i = 0; i < 100; i++ )
   {
      mpTrackArtist[i]->setText( artist );
      mpTrackArtist[i]->setCursorPosition( 1 );
   }
}


void CDEdit::normalizeArtist()
{
   if( mpDiscArtist->isEnabled() )
   {
      QString artist( mpDiscArtist->text() );
      artist = TagList::normalizeString( artist );
      mpDiscArtist->setText( artist );
      mpDiscArtist->setCursorPosition( 0 );
   }
   for( int i = 0; i < 100; i++ )
   {
      if( mpTrackArtist[i]->isEnabled() )
      {
         mpTrackArtist[i]->setText( TagList::normalizeString( mpTrackArtist[i]->text() ) );
         mpTrackArtist[i]->setCursorPosition( 0 );
      }
   }
}


void CDEdit::normalizeTitle()
{
   if( mpDiscTitle->isEnabled() )
   {
      QString title( mpDiscTitle->text() );
      /* protect the term "CD" from turning into "Cd" */
      title.replace( "CD", "\x01\x01" );
      title = TagList::normalizeString( title );
      title.replace( "\x01\x01", "CD" );
      mpDiscTitle->setText( title );
      mpDiscTitle->setCursorPosition( 0 );
   }
   for( int i = 0; i < 100; i++ )
   {
      if( mpTrackTitle[i]->isEnabled() )
      {
         mpTrackTitle[i]->setText( TagList::normalizeString( mpTrackTitle[i]->text() ) );
         mpTrackTitle[i]->setCursorPosition( 0 );
      }
   }
}


void CDEdit::copyYear()
{
   int i=0;
   QString year;

   if( mpTrackNr[0]->isHidden() )
   {
      i++;
   }

   year = mpTrackYear[i]->text();

   while( ++i < 100 )
   {
      mpTrackYear[i]->setText( year );
   }
}


void CDEdit::setTrackHidden( int track, bool hide )
{
   mpTrackNr[track]->setHidden( hide );
   mpEnqueueTrack[track]->setHidden( hide );
   mpTrackArtist[track]->setHidden( hide );
   mpTrackTitle[track]->setHidden( hide );
   mpTrackYear[track]->setHidden( hide );
   mpTrackPlaytime[track]->setHidden( hide );
}


void CDEdit::setTrackDisabled( int track, bool disable )
{
   if( track < 0 )
   {
      mpDiscArtist->setDisabled( disable );
      mpDiscTitle->setDisabled( disable );
      mpDiscGenre->setDisabled( disable );
      return;
   }

   /* everything that's not an audio track is always disabled */
   if( !mpCDInfo->isAudio( track ) )
   {
      disable = true;
   }
   mpTrackNr[track]->setDisabled( disable );
   mpEnqueueTrack[track]->setDisabled( disable );
   mpTrackArtist[track]->setDisabled( disable );
   mpTrackTitle[track]->setDisabled( disable );
   mpTrackYear[track]->setDisabled( disable && (track != (mpTrackYear[0]->isHidden() ? 1 : 0)) );
   mpTrackPlaytime[track]->setDisabled( disable );
}


void CDEdit::update()
{
   int i = 0;
   int count = 4;
   QString length;

   mpDiscArtist->setText( mpCDInfo->artist() );
   mpDiscTitle->setText( mpCDInfo->title() );
   mpDiscGenre->setText( mpCDInfo->genre() );
   mpDiscID->setText( mpCDInfo->cddbDiscID() );
   mpDiscPlaytime->setText( mpCDInfo->length() );
   for( i = 0; i < 100; i++ )
   {
      length = mpCDInfo->length( i );
      mpTrackPlaytime[i]->setText( length );
      mpEnqueueTrack[i]->setChecked( false );
      mpTrackNr[i]->setChecked( (i != 0) && mpCDInfo->isAudio(i) );
      setTrackDisabled( i, !mpCDInfo->isAudio(i) );
      bool empty = length.isEmpty();
      setTrackHidden( i, empty );
      mpTrackArtist[i]->setText( mpCDInfo->artist(i) );
      mpTrackTitle[i]->setText( mpCDInfo->title(i) );
      if( mpCDInfo->year() <= 0 )
      {
         mpTrackYear[i]->setText( QString() );
      }
      else
      {
         mpTrackYear[i]->setText( QString::number( mpCDInfo->year() ) );
      }
      if( !empty )
      {
         count++;
      }
   }

   if( mpTrackYear[0]->isHidden() )
   {
      mpTrackYear[1]->setDisabled( false );
   }
   else
   {
      mpTrackYear[0]->setDisabled( false );
   }
   mpScrollWidget->resize( mpScrollArea->width() - 20, count * 24 );

   emit containsData( true );
}


void CDEdit::updateCDText( int track, const QString &artist, const QString &title )
{
   if( !track )
   {
      mpDiscArtist->setText( artist );
      mpDiscTitle->setText( title );
   }
   else
   {
      mpTrackArtist[track]->setText( artist );
      mpTrackTitle[track]->setText( title );
      mpTrackArtist[track]->setCursorPosition( 1 );
      mpTrackTitle[track]->setCursorPosition( 1 );
   }
}


void CDEdit::trackInfo( int tracknr, bool *dorip, bool *doenqueue, QString *artist, QString *title,
                        QString *albumartist, QString *albumtitle, QString *genre, int *year )
{
   if( mpTrackNr[tracknr]->isHidden() ||
       (mpTrackNr[tracknr]->checkState() == Qt::Unchecked) )
   {
      *dorip = false;
      *doenqueue = false;
      artist->clear();
      title->clear();
      albumartist->clear();
      albumtitle->clear();
      genre->clear();
      *year = 0;
   }
   else
   {
      *dorip        = true;
      *doenqueue    = mpEnqueueTrack[tracknr]->isChecked();
      *artist       = mpTrackArtist[tracknr]->text();
      *title        = mpTrackTitle[tracknr]->text();
      *albumartist  = mpDiscArtist->text();
      *albumtitle   = mpDiscTitle->text();
      *genre        = mpDiscGenre->text();
      *year         = mpTrackYear[tracknr]->text().toLong();
   }
}


void CDEdit::ensureVisible( int track )
{
   if( mpTrackTitle[track]->isVisible() )
   {
      mpScrollArea->ensureWidgetVisible( mpTrackTitle[track], 5, 5 );
   }
}


void CDEdit::ensureVisibleFocus( QWidget *widget )
{
   if( widget->isVisible() )
   {
      widget->setFocus();
      mpScrollArea->ensureWidgetVisible( widget, 5, 5 );
   }
}


void CDEdit::keyUpDown( QWidget *widget, bool isUp )
{
   int firstLine = mpTrackNr[0]->isVisible() ? 0 : 1;
   if( widget == mpDiscArtist )
   {
      if( !isUp )
      {
         ensureVisibleFocus( mpDiscTitle );
      }
      return;
   }
   if( widget == mpDiscTitle )
   {
      if( isUp )
      {
         ensureVisibleFocus( mpDiscArtist );
      }
      else
      {
         ensureVisibleFocus( mpDiscGenre );
      }
      return;
   }
   if( widget == mpDiscGenre )
   {
      if( isUp )
      {
         ensureVisibleFocus( mpDiscTitle );
      }
      else
      {
         switch( mLastColumn )
         {
            case 0:
               ensureVisibleFocus( mpTrackNr[firstLine] );
               break;
            case 1:
               ensureVisibleFocus( mpEnqueueTrack[firstLine] );
               break;
            case 2:
               ensureVisibleFocus( mpTrackArtist[firstLine] );
               break;
            case 3:
               ensureVisibleFocus( mpTrackTitle[firstLine] );
               break;
            case 4:
               ensureVisibleFocus( mpTrackYear[firstLine] );
               break;
            default:
               break;
         }
      }
      return;
   }

   if( isUp && (mpTrackNr[firstLine] == widget) )
   {
      mLastColumn = 0;
      ensureVisibleFocus( mpDiscGenre );
      return;
   }
   if( isUp && (mpEnqueueTrack[firstLine] == widget) )
   {
      mLastColumn = 1;
      ensureVisibleFocus( mpDiscGenre );
      return;
   }
   if( isUp && (mpTrackArtist[firstLine] == widget) )
   {
      mLastColumn = 2;
      ensureVisibleFocus( mpDiscGenre );
      return;
   }
   if( isUp && (mpTrackTitle[firstLine] == widget) )
   {
      mLastColumn = 3;
      ensureVisibleFocus( mpDiscGenre );
      return;
   }
   if( isUp && (mpTrackYear[firstLine] == widget) )
   {
      mLastColumn = 4;
      ensureVisibleFocus( mpDiscGenre );
      return;
   }
   for( int i = isUp ? 1 : 0; i < (isUp ? 100 : 99); i++ )
   {
      if( mpTrackNr[i] == widget )
      {
         ensureVisibleFocus( mpTrackNr[isUp ? i-1 : i+1] );
         return;
      }
      if( mpEnqueueTrack[i] == widget )
      {
         ensureVisibleFocus( mpEnqueueTrack[isUp ? i-1 : i+1] );
         return;
      }
      if( mpTrackArtist[i] == widget )
      {
         ensureVisibleFocus( mpTrackArtist[isUp ? i-1 : i+1] );
         return;
      }
      if( mpTrackTitle[i] == widget )
      {
         ensureVisibleFocus( mpTrackTitle[isUp ? i-1 : i+1] );
         return;
      }
      if( mpTrackYear[i] == widget )
      {
         ensureVisibleFocus( mpTrackYear[isUp ? i-1 : i+1] );
         return;
      }
   }
}
