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
, mpArtist( new ScrollLine( this ) )
, mpTitle( new ScrollLine( this ) )
, mpAlbum( new ScrollLine( this ) )
, mpFavoriteButton( new QPushButton( tr("Favorite") ) )
, mpUnwantedButton( new QPushButton( tr("No Auto") ) )
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
   
   mainLayout->addWidget( mpArtistLabel, 0, 0 );
   mainLayout->addWidget( mpTitleLabel,  1, 0 );
   mainLayout->addWidget( mpAlbumLabel,  2, 0 );
   
   mainLayout->addWidget( mpArtist,      0, 1, 1, 4 );
   mainLayout->addWidget( mpTitle,       1, 1, 1, 4 );
   mainLayout->addWidget( mpAlbum,       2, 1, 1, 4 );
   
   mainLayout->setRowStretch( 3, 1 );
   
   mainLayout->addWidget( mpFavoriteButton, 4, 1 );
   mainLayout->addWidget( mpUnwantedButton, 4, 3 );
   
   connect( mpFavoriteButton, SIGNAL(clicked()),
            this, SLOT(handleFavoriteButton()) );
   connect( mpUnwantedButton, SIGNAL(clicked()),
            this, SLOT(handleUnwantedButton()) );
}


void TrackInfoWidget::handleFavoriteButton()
{
   if( mpFavoriteButton->isChecked() )
   {
      mpUnwantedButton->setChecked( false );
   }
}


void TrackInfoWidget::handleUnwantedButton()
{
   if( mpUnwantedButton->isChecked() )
   {
      mpFavoriteButton->setChecked( false );
   }
}


void TrackInfoWidget::getTrack( const QString &fileName )
{
   if( mpDatabase->getTrackInfo( &mTrackInfo, fileName ) )
   {
      mpArtist->setText( mTrackInfo.mArtist );
      mpTitle->setText( mTrackInfo.mTitle );
      mpAlbum->setText( mTrackInfo.mAlbum );
   }
   else
   {
      QString empty;
      
      mpArtist->setText( empty );
      mpTitle->setText( empty );
      mpAlbum->setText( empty );
   }
}
