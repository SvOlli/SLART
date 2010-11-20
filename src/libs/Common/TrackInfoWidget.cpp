/**
 * src/libs/Common/TrackInfoWidget.Cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "Database.hpp"
#include "MySettings.hpp"
#include "TrackInfoWidget.hpp"
#include "Satellite.hpp"
#include "ScrollLine.hpp"

#include <QtGui>

#include "Trace.hpp"

TrackInfoWidget::TrackInfoWidget( Database *database, const QByteArray &updateCode,
                                  bool includeFolders, QWidget *parent )
: QWidget( parent )
, mpDatabase( database )
, mTrackInfo()
, mUpdateCode( updateCode )
, mIncludeFolders( includeFolders )
, mpTimesPlayed( new QLabel( this ) )
, mpArtist( new ScrollLine( this ) )
, mpTitle( new ScrollLine( this ) )
, mpAlbum( new ScrollLine( this ) )
, mpTrackNr( new ScrollLine( this, false ) )
, mpFavoriteButton( new QCheckBox( tr("Favorite") ) )
, mpUnwantedButton( new QCheckBox( tr("No Auto") ) )
{
   mpFavoriteButton->setCheckable( true );
   mpUnwantedButton->setCheckable( true );
   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
   mainLayout->setVerticalSpacing( 0 );
   mainLayout->setHorizontalSpacing( 1 );

   mainLayout->addWidget( new QLabel( tr("Artist:"), this ), 1, 0 );
   mainLayout->addWidget( new QLabel( tr("Title:"), this ),  2, 0 );
   mainLayout->addWidget( new QLabel( tr("Album:"), this ),  3, 0 );
   mainLayout->addWidget( new QLabel( tr("Track:"), this ),  4, 0 );

   mainLayout->addWidget( mpArtist,         1, 1, 1, 4 );
   mainLayout->addWidget( mpTitle,          2, 1, 1, 4 );
   mainLayout->addWidget( mpAlbum,          3, 1, 1, 4 );
   mainLayout->addWidget( mpTrackNr,        4, 1 );
   mainLayout->addWidget( mpTimesPlayed,    4, 2 );
   mainLayout->addWidget( mpFavoriteButton, 4, 3 );
   mainLayout->addWidget( mpUnwantedButton, 4, 4 );

   mainLayout->setColumnStretch( 0,  1 );
   mainLayout->setColumnStretch( 1, 17 );
   mainLayout->setColumnStretch( 2, 80 );
   mainLayout->setColumnStretch( 3,  1 );
   mainLayout->setColumnStretch( 4,  1 );

   mainLayout->setRowStretch( 0, 1 );
   mainLayout->setRowStretch( 1, 0 );
   mainLayout->setRowStretch( 2, 0 );
   mainLayout->setRowStretch( 3, 0 );
   mainLayout->setRowStretch( 4, 0 );
   mainLayout->setRowStretch( 5, 1 );

   mpTimesPlayed->setAlignment( Qt::AlignCenter );

   connect( mpFavoriteButton, SIGNAL(clicked()),
            this, SLOT(handleFavoriteButton()) );
   connect( mpUnwantedButton, SIGNAL(clicked()),
            this, SLOT(handleUnwantedButton()) );

   update( false );
}


void TrackInfoWidget::handleFavoriteButton()
{
   bool checked = mpFavoriteButton->isChecked();
   if( checked )
   {
      mpUnwantedButton->setChecked( false );
   }
   mTrackInfo.setFlag( TrackInfo::Favorite, checked );
   emit checkboxClicked( mTrackInfo );
   mpDatabase->updateTrackInfo( &mTrackInfo );
   Satellite::send1( mUpdateCode );
}


void TrackInfoWidget::handleUnwantedButton()
{
   bool checked = mpUnwantedButton->isChecked();
   if( checked )
   {
      mpFavoriteButton->setChecked( false );
   }
   mTrackInfo.setFlag( TrackInfo::Unwanted, checked );
   emit checkboxClicked( mTrackInfo );
   mpDatabase->updateTrackInfo( &mTrackInfo );
   Satellite::send1( mUpdateCode );
}


void TrackInfoWidget::setFavoriteUnwanted( bool favorite, bool unwanted )
{
   mTrackInfo.setFlag( TrackInfo::Unwanted, false );
   if( favorite && unwanted )
   {
      favorite = false;
      unwanted = false;
   }
   mpFavoriteButton->setChecked( favorite );
   mpUnwantedButton->setChecked( unwanted );
   if( favorite )
   {
      mTrackInfo.setFlag( TrackInfo::Favorite, true );
   }
   if( unwanted )
   {
      mTrackInfo.setFlag( TrackInfo::Unwanted, true );
   }
   mpDatabase->updateTrackInfo( &mTrackInfo );
   Satellite::send1( mUpdateCode );
}


void TrackInfoWidget::getTrack( const TrackInfo &trackInfo )
{
   mTrackInfo = trackInfo;
   update( false );
}


void TrackInfoWidget::update( bool reread )
{
   if( reread )
   {
      mpDatabase->getTrackInfo( &mTrackInfo );
   }
   mpFavoriteButton->setChecked( false );
   mpUnwantedButton->setChecked( false );
   if( mTrackInfo.mID )
   {
      setDisabled( false );
      QString folderToolTip( mTrackInfo.getFolders().join("\n") );
      mpArtist->setText( mTrackInfo.mArtist );
      mpTitle->setText( mTrackInfo.mTitle );
      mpAlbum->setText( mTrackInfo.mAlbum );
      if( mIncludeFolders )
      {
         mpFavoriteButton->setToolTip( folderToolTip );
         mpUnwantedButton->setToolTip( folderToolTip );
      }
      if( mTrackInfo.mTrackNr < 0 )
      {
         mpTrackNr->clear();
      }
      else
      {
         mpTrackNr->setText( QString::number(mTrackInfo.mTrackNr) );
      }
      if( mTrackInfo.mTimesPlayed == 1 )
      {
         mpTimesPlayed->setText( QString::number(mTrackInfo.mTimesPlayed)+" Time Played" );
      }
      else
      {
         mpTimesPlayed->setText( QString::number(mTrackInfo.mTimesPlayed)+" Times Played" );
      }

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

      setDisabled( true );
      mpArtist->setText( empty );
      mpTitle->setText( empty );
      mpAlbum->setText( empty );
      mpTrackNr->setText( empty );
      mpTimesPlayed->setText( empty );
      mpFavoriteButton->setToolTip( empty );
      mpUnwantedButton->setToolTip( empty );
   }
}
