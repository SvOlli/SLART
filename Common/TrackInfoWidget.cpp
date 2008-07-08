/**
 * TrackInfoWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "TrackInfoWidget.hpp"
#include "ScrollLine.hpp"
#include "Database.hpp"

#include <QtGui>

#include "Trace.hpp"

TrackInfoWidget::TrackInfoWidget( Database *database, QWidget *parent )
: QWidget( parent )
, mpDatabase( database )
, mTrackInfo()
, mpArtistLabel( new QLabel( tr("Artist:"), this ) )
, mpTitleLabel( new QLabel( tr("Title:"), this ) )
, mpAlbumLabel( new QLabel( tr("Album:"), this ) )
, mpTrackNrLabel( new QLabel( tr("Track:"), this ) )
, mpTimesPlayed( new QLabel( this ) )
, mpArtist( new ScrollLine( this ) )
, mpTitle( new ScrollLine( this ) )
, mpAlbum( new ScrollLine( this ) )
, mpTrackNr( new QLineEdit( this ) )
, mpFavoriteButton( new QCheckBox( tr("Favorite") ) )
, mpUnwantedButton( new QCheckBox( tr("No Auto") ) )
{
   mpFavoriteButton->setCheckable( true );
   mpUnwantedButton->setCheckable( true );
   QGridLayout *mainLayout = new QGridLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 1 );
#else
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
   mainLayout->setVerticalSpacing( 0 );
   mainLayout->setHorizontalSpacing( 1 );
#endif
   
   mainLayout->addWidget( mpArtistLabel,    0, 0 );
   mainLayout->addWidget( mpArtist,         0, 1, 1, 4 );
   mainLayout->addWidget( mpTitleLabel,     1, 0 );
   mainLayout->addWidget( mpTitle,          1, 1, 1, 4 );
   mainLayout->addWidget( mpAlbumLabel,     2, 0 );
   mainLayout->addWidget( mpAlbum,          2, 1, 1, 4 );
   mainLayout->addWidget( mpTrackNrLabel,   3, 0 );
   mainLayout->addWidget( mpTrackNr,        3, 1 );
   mainLayout->addWidget( mpTimesPlayed,    3, 2 );
   mainLayout->addWidget( mpFavoriteButton, 3, 3 );
   mainLayout->addWidget( mpUnwantedButton, 3, 4 );
   
   mainLayout->setColumnStretch( 0,  1 );
   mainLayout->setColumnStretch( 1, 17 );
   mainLayout->setColumnStretch( 2, 80 );
   mainLayout->setColumnStretch( 3,  1 );
   mainLayout->setColumnStretch( 4,  1 );
   
   mpTrackNr->setReadOnly( true );
   mpTimesPlayed->setAlignment( Qt::AlignCenter );
   
   connect( mpFavoriteButton, SIGNAL(clicked()),
            this, SLOT(handleFavoriteButton()) );
   connect( mpUnwantedButton, SIGNAL(clicked()),
            this, SLOT(handleUnwantedButton()) );
}


void TrackInfoWidget::handleFavoriteButton()
{
   bool checked = mpFavoriteButton->isChecked();
   if( checked )
   {
      mpUnwantedButton->setChecked( false );
   }
   mTrackInfo.setFlag( TrackInfo::Favorite, checked );
   mpDatabase->updateTrackInfo( &mTrackInfo );
}


void TrackInfoWidget::handleUnwantedButton()
{
   bool checked = mpUnwantedButton->isChecked();
   if( checked )
   {
      mpFavoriteButton->setChecked( false );
   }
   mTrackInfo.setFlag( TrackInfo::Unwanted, checked );
   mpDatabase->updateTrackInfo( &mTrackInfo );
}


void TrackInfoWidget::getTrack( const TrackInfo &trackInfo )
{
   mTrackInfo = trackInfo;
   mpFavoriteButton->setChecked( false );
   mpUnwantedButton->setChecked( false );
   if( mTrackInfo.mID )
   {
      mpArtist->setText( mTrackInfo.mArtist );
      mpTitle->setText( mTrackInfo.mTitle );
      mpAlbum->setText( mTrackInfo.mAlbum );
      mpTrackNr->setText( QString::number(mTrackInfo.mTrackNr) );
      if( mTrackInfo.mTimesPlayed == 1 )
      {
         mpTimesPlayed->setText( QString::number(mTrackInfo.mTimesPlayed)+" Time Played" );
      }
      else
      {
         mpTimesPlayed->setText( QString::number(mTrackInfo.mTimesPlayed)+" Times Played" );
      }
      
      mpFavoriteButton->setDisabled( false );
      mpUnwantedButton->setDisabled( false );
      if( mTrackInfo.isFlagged( TrackInfo::Favorite ) )
      {
         mpFavoriteButton->setChecked( true );
      }
      if( mTrackInfo.isFlagged( TrackInfo::Unwanted ) )
      {
         mpUnwantedButton->setChecked( true );
      }
   }
   else
   {
      QString empty;
      
      mpArtist->setText( empty );
      mpTitle->setText( empty );
      mpAlbum->setText( empty );
      mpTrackNr->setText( empty );
      mpTimesPlayed->setText( empty );
      mpFavoriteButton->setDisabled( true );
      mpUnwantedButton->setDisabled( true );
   }
}