/**
 * CDEdit.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "CDEdit.hpp"
#include "CDToc.hpp"
#include "CDDB.hpp"
#include "TagList.hpp"

#include <QtGui>
#include "Trace.hpp"


CDEdit::CDEdit( CDToc *toc, CDDB *cddb, QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpToc( toc )
, mpCDDB( cddb )
, mpScrollArea( new QScrollArea( this ) )
, mpScrollWidget( new QWidget( this ) )
, mpMainLayout( new QGridLayout( mpScrollWidget ) )
, mpLabelDiscArtist( new QLabel( tr("Artist:"), this ) )
, mpLabelDiscTitle( new QLabel( tr("Title:"), this ) )
, mpLabelGenre( new QLabel( tr("Genre:"), this ) )
, mpDiscArtist( new QLineEdit( this ) )
, mpDiscTitle( new QLineEdit( this ) )
, mpGenre( new QLineEdit( this ) )
, mpLabelDiscID( new QLabel( tr("DiscID"), this ) )
, mpDiscID( new QLabel( this ) )
, mpLabelTrackNr( new QLabel( tr("Nr"), this ) )
, mpLabelEnqueueTrack( new QLabel( tr("NQ"), this ) )
, mpLabelTrackArtist( new QLabel( tr("Artist"), this ) )
, mpLabelTrackTitle( new QLabel( tr("Title"), this ) )
, mpLabelTrackYear( new QLabel( tr("Year"), this ) )
, mpLabelTrackPlaytime( new QLabel( tr("Playtime"), this ) )
, mpSplitMode( new QComboBox( this ) )
, mpSplitButton( new QPushButton( tr("Split Titles"), this ) )
, mpToggleRipButton( new QPushButton( tr("Toggle Rip"), this ) )
, mpToggleEnqueueButton( new QPushButton( tr("Toggle Enqueue"), this ) )
, mpCopyArtistButton( new QPushButton( tr("Copy Artist"), this ) )
, mpNormalizeTitleButton( new QPushButton( tr("Norm. Title"), this ) )
, mpCopyYearButton( new QPushButton( tr("Copy Year"), this ) )
{
   QVBoxLayout *outerLayout  = new QVBoxLayout( this );
   QHBoxLayout *buttonLayout = new QHBoxLayout();
   
#if QT_VERSION < 0x040300
   outerLayout->setMargin( 0 );
#else
   outerLayout->setContentsMargins( 0, 0, 0, 0 );
#endif
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

   mpTrackNr       = new QCheckBox*[100];
   mpEnqueueTrack  = new QCheckBox*[100];
   mpTrackArtist   = new QLineEdit*[100];
   mpTrackTitle    = new QLineEdit*[100];
   mpTrackYear     = new QLineEdit*[100];
   mpTrackPlaytime = new QLabel*[100];
   
   mpMainLayout->setSpacing( 1 );
   mpMainLayout->setColumnStretch( 2, 1 );
   mpMainLayout->setColumnStretch( 3, 1 );
   mpDiscArtist->setText( tr("DiscArtist" ) );
   mpDiscTitle->setText( tr("DiscTitle" ) );
   
   mpMainLayout->addWidget( mpLabelDiscArtist, 0, 0 );
   mpMainLayout->addWidget( mpDiscArtist,      0, 1, 1, 4 );
   mpMainLayout->addWidget( mpLabelDiscTitle,  1, 0 );
   mpMainLayout->addWidget( mpDiscTitle,       1, 1, 1, 4 );
   mpMainLayout->addWidget( mpLabelGenre,      2, 0 );
   mpMainLayout->addWidget( mpGenre,           2, 1, 1, 4 );
   mpMainLayout->addWidget( mpLabelDiscID,     0, 5 );
   mpMainLayout->addWidget( mpDiscID,          1, 5 );
   
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
         mpTrackNr[i]       = new QCheckBox( QString("  ")+QString::number(i), this );
      }
      else
      {
         mpTrackNr[i]       = new QCheckBox( QString::number(i), this );
      }
      mpEnqueueTrack[i]  = new QCheckBox( this );
      mpTrackArtist[i]   = new QLineEdit( this );
      mpTrackTitle[i]    = new QLineEdit( this );
      mpTrackYear[i]     = new QLineEdit( this );
      mpTrackPlaytime[i] = new QLabel( this );
      mpTrackPlaytime[i]->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
      
      mpMainLayout->addWidget( mpTrackNr[i],       4+i, 0 );
      mpMainLayout->addWidget( mpEnqueueTrack[i]  ,4+i, 1 );
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
   
   mpSplitMode->addItem(" / ");
   mpSplitMode->addItem(" - ");
   mpSplitMode->addItem("/");
   mpSplitMode->addItem("-");
   buttonLayout->addWidget( mpSplitMode );
   buttonLayout->addWidget( mpSplitButton );
   buttonLayout->addWidget( mpToggleRipButton );
   buttonLayout->addWidget( mpToggleEnqueueButton );
   buttonLayout->addWidget( mpCopyArtistButton );
   buttonLayout->addWidget( mpNormalizeTitleButton );
   buttonLayout->addWidget( mpCopyYearButton );
   mpSplitMode->setMinimumHeight( mpSplitButton->minimumHeight() );

   mpScrollWidget->setLayout( mpMainLayout );
   mpScrollArea->setWidget( mpScrollWidget );
   mpScrollArea->setWidgetResizable( true );
   outerLayout->addWidget( mpScrollArea );
   outerLayout->addLayout( buttonLayout );
   
   connect( mpToggleRipButton,      SIGNAL(clicked()), this, SLOT(handleTrackNr()) );
   connect( mpToggleEnqueueButton,  SIGNAL(clicked()), this, SLOT(handleEnqueueTrack()) );
   connect( mpCopyArtistButton,     SIGNAL(clicked()), this, SLOT(handleTrackArtist()) );
   connect( mpNormalizeTitleButton, SIGNAL(clicked()), this, SLOT(handleNormalizeTitle()) );
   connect( mpCopyYearButton,       SIGNAL(clicked()), this, SLOT(handleTrackYear()) );
   connect( mpSplitButton,          SIGNAL(clicked()), this, SLOT(splitTitles()) );
   
   setLayout( outerLayout );
}


void CDEdit::clear()
{
   mpDiscArtist->clear();
   mpDiscTitle->clear();
   mpGenre->clear();
   
   for( int i = 0; i < 100; i++ )
   {
      mpTrackArtist[i]->clear();
      mpTrackTitle[i]->clear();
      mpTrackYear[i]->clear();
      setTrackHidden( i, true );
   }
}


void CDEdit::handleTrackNr()
{
   for( int i = 1; i < 100; i++ )
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


void CDEdit::handleEnqueueTrack()
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


void CDEdit::handleTrackArtist()
{
   QString artist( mpDiscArtist->text() );
   
   for( int i = 0; i < 100; i++ )
   {
      mpTrackArtist[i]->setText( artist );
   }
}


void CDEdit::handleNormalizeTitle()
{
   mpDiscTitle->setText( TagList::normalizeString( mpDiscTitle->text() ) );
   for( int i = 0; i < 100; i++ )
   {
      mpTrackTitle[i]->setText( TagList::normalizeString( mpTrackTitle[i]->text() ) );
   }
}


void CDEdit::handleTrackYear()
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
   mpTrackNr[track]->setDisabled( disable );
   mpEnqueueTrack[track]->setDisabled( disable );
   mpTrackArtist[track]->setDisabled( disable );
   mpTrackTitle[track]->setDisabled( disable );
   mpTrackYear[track]->setDisabled( disable );
   mpTrackPlaytime[track]->setDisabled( disable );
}


void CDEdit::update()
{
   int i, count = 4;
   QString length;

   mpCDDB->query( mpToc->query() );
   
   for( i = 0; i < 100; i++ )
   {
      length = mpToc->length( i );
      mpTrackPlaytime[i]->setText( length );
      mpEnqueueTrack[i]->setChecked( false );
      bool empty = length.isEmpty();
      setTrackHidden( i, empty );
      if( !empty )
      {
         count++;
      }
   }

   mpScrollWidget->resize( mpScrollArea->width() - 20, count * 24 );
   mpDiscID->setText( mpToc->cddbDiscID() );
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
   }
}


void CDEdit::updateCDDB()
{
   int i;
   mpDiscTitle->setText( mpToc->mTitle[0] );
   mpGenre->setText( mpToc->mGenre );
   
   mpTrackYear[0]->setText( QString::number( mpToc->mYear ) );
   
   for( i = 1; i < 100; i++ )
   {
      mpTrackTitle[i]->setText( mpToc->mTitle[i] );
      mpTrackYear[i]->setText( QString::number( mpToc->mYear ) );
   }
   
   splitTitles();
}


void CDEdit::splitTitles()
{
   QString splitMode( mpSplitMode->currentText() );
   QString title( mpDiscTitle->text() );
   int i;
   int indexOfSplit = title.indexOf( splitMode );
   
   if( indexOfSplit > 0 )
   {
      mpDiscArtist->setText( title.left(indexOfSplit) );
      mpDiscTitle->setText( title.mid( indexOfSplit + splitMode.size() ) );
   }

   for( i = 0; i < 100; i++ )
   {
      title = mpTrackTitle[i]->text();
      indexOfSplit = title.indexOf( splitMode );
      
      if( indexOfSplit > 0 )
      {
         mpTrackArtist[i]->setText( title.left(indexOfSplit) );
         mpTrackTitle[i]->setText( title.mid( indexOfSplit + splitMode.size() ) );
      }
      else
      {
         mpTrackArtist[i]->setText( mpDiscArtist->text() );
      }
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
      *genre        = mpGenre->text();
      *year         = mpTrackYear[tracknr]->text().toLong();
   }
}


void CDEdit::ensureVisible( int tracknr )
{
   mpScrollArea->ensureWidgetVisible( mpTrackTitle[tracknr], 1, 1 );
}
