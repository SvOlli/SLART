/**
 * CDEdit.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "CDEdit.hpp"
#include "CDToc.hpp"
#include "CDDB.hpp"


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
, mpLabelTrackArtist( new QLabel( tr("Artist"), this ) )
, mpLabelTrackTitle( new QLabel( tr("Title"), this ) )
, mpLabelTrackYear( new QLabel( tr("Year"), this ) )
, mpLabelTrackPlaytime( new QLabel( tr("Playtime"), this ) )
, mpSplitMode( new QComboBox( this ) )
, mpSplitButton( new QPushButton( tr("Split Titles"), this ) )
, mpToggleAllButton( new QPushButton( tr("Toggle All"), this ) )
, mpCopyArtistButton( new QPushButton( tr("Copy Artist"), this ) )
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
   mpLabelTrackArtist->setFrameShape( QFrame::Box );
   mpLabelTrackTitle->setFrameShape( QFrame::Box );
   mpLabelTrackYear->setFrameShape( QFrame::Box );
   mpLabelTrackPlaytime->setFrameShape( QFrame::Box );

   mpTrackNr       = new QCheckBox*[100];
   mpTrackArtist   = new QLineEdit*[100];
   mpTrackTitle    = new QLineEdit*[100];
   mpTrackYear     = new QLineEdit*[100];
   mpTrackPlaytime = new QLabel*[100];
   
   mpMainLayout->setSpacing( 1 );
   mpMainLayout->setColumnStretch( 1, 1 );
   mpMainLayout->setColumnStretch( 2, 1 );
   mpDiscArtist->setText( tr("DiscArtist" ) );
   mpDiscTitle->setText( tr("DiscTitle" ) );
   
   mpMainLayout->addWidget( mpLabelDiscArtist, 0, 0 );
   mpMainLayout->addWidget( mpDiscArtist,      0, 1, 1, 3 );
   mpMainLayout->addWidget( mpLabelDiscTitle,  1, 0 );
   mpMainLayout->addWidget( mpDiscTitle,       1, 1, 1, 3 );
   mpMainLayout->addWidget( mpLabelGenre,      2, 0 );
   mpMainLayout->addWidget( mpGenre,           2, 1, 1, 3 );
   mpMainLayout->addWidget( mpLabelDiscID,     0, 4 );
   mpMainLayout->addWidget( mpDiscID,          1, 4 );
   
   mpMainLayout->addWidget( mpLabelTrackNr,       3, 0 );
   mpMainLayout->addWidget( mpLabelTrackArtist,   3, 1 );
   mpMainLayout->addWidget( mpLabelTrackTitle,    3, 2 );
   mpMainLayout->addWidget( mpLabelTrackYear,     3, 3 );
   mpMainLayout->addWidget( mpLabelTrackPlaytime, 3, 4 );
   
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
      mpTrackArtist[i]   = new QLineEdit( this );
      mpTrackTitle[i]    = new QLineEdit( this );
      mpTrackYear[i]     = new QLineEdit( this );
      mpTrackPlaytime[i] = new QLabel( this );
      mpTrackPlaytime[i]->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
      
      mpMainLayout->addWidget( mpTrackNr[i],       4+i, 0 );
      mpMainLayout->addWidget( mpTrackArtist[i],   4+i, 1 );
      mpMainLayout->addWidget( mpTrackTitle[i],    4+i, 2 );
      mpMainLayout->addWidget( mpTrackYear[i],     4+i, 3 );
      mpMainLayout->addWidget( mpTrackPlaytime[i], 4+i, 4 );
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
   buttonLayout->addWidget( mpToggleAllButton );
   buttonLayout->addWidget( mpCopyArtistButton );
   buttonLayout->addWidget( mpCopyYearButton );
   mpSplitMode->setMinimumHeight( mpSplitButton->minimumHeight() );

   mpScrollWidget->setLayout( mpMainLayout );
   mpScrollArea->setWidget( mpScrollWidget );
   mpScrollArea->setWidgetResizable( true );
   outerLayout->addWidget( mpScrollArea );
   outerLayout->addLayout( buttonLayout );
   
   connect( mpToggleAllButton,  SIGNAL(clicked()), this, SLOT(handleTrackNr()) );
   connect( mpCopyArtistButton, SIGNAL(clicked()), this, SLOT(handleTrackArtist()) );
   connect( mpCopyYearButton,   SIGNAL(clicked()), this, SLOT(handleTrackYear()) );
   connect( mpSplitButton,      SIGNAL(clicked()), this, SLOT(splitTitles()) );
   
   setLayout( outerLayout );
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


void CDEdit::handleTrackArtist()
{
   QString artist( mpDiscArtist->text() );
   
   for( int i = 0; i < 100; i++ )
   {
      mpTrackArtist[i]->setText( artist );
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
   mpTrackArtist[track]->setHidden( hide );
   mpTrackTitle[track]->setHidden( hide );
   mpTrackYear[track]->setHidden( hide );
   mpTrackPlaytime[track]->setHidden( hide );
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


void CDEdit::trackInfo( int tracknr, bool *dorip, QString *artist, QString *title,
                        QString *albumartist, QString *albumtitle, QString *genre, int *year )
{
   if( mpTrackNr[tracknr]->isHidden() ||
       (mpTrackNr[tracknr]->checkState() == Qt::Unchecked) )
   {
      *dorip = false;
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
      *artist       = mpTrackArtist[tracknr]->text();
      *title        = mpTrackTitle[tracknr]->text();
      *albumartist  = mpDiscArtist->text();
      *albumtitle   = mpDiscTitle->text();
      *genre        = mpGenre->text();
      *year         = mpTrackYear[tracknr]->text().toLong();
   }
}
