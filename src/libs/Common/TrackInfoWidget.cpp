/*
 * src/libs/Common/TrackInfoWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "Database.hpp"
#include "DatabaseInterface.hpp"
#include "TrackInfoWidget.hpp"
#include "Satellite.hpp"
#include "ScrollLine.hpp"

#include <QtGui>

#include "Trace.hpp"


TrackInfoWidget::TrackInfoWidget( bool includeFolders, QWidget *parent )
: QWidget( parent )
, mpDatabase( 0 )
, mTrackInfo()
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

   if( includeFolders )
   {
      mpFavoriteButton->setContextMenuPolicy( Qt::CustomContextMenu );
      mpUnwantedButton->setContextMenuPolicy( Qt::CustomContextMenu );

      connect( mpFavoriteButton, SIGNAL(customContextMenuRequested(QPoint)),
               this, SLOT(handleGroupsMenu()) );
      connect( mpUnwantedButton, SIGNAL(customContextMenuRequested(QPoint)),
               this, SLOT(handleGroupsMenu()) );
   }

   update( false );
}


TrackInfoWidget::~TrackInfoWidget()
{
}


TrackInfoWidget::TrackInfoWidget( Database *database,
                                  bool includeFolders, QWidget *parent )
: QWidget( parent )
, mpDatabase( database )
, mTrackInfo()
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

   if( includeFolders )
   {
      mpFavoriteButton->setContextMenuPolicy( Qt::CustomContextMenu );
      mpUnwantedButton->setContextMenuPolicy( Qt::CustomContextMenu );

      connect( mpFavoriteButton, SIGNAL(customContextMenuRequested(QPoint)),
               this, SLOT(handleGroupsMenu()) );
      connect( mpUnwantedButton, SIGNAL(customContextMenuRequested(QPoint)),
               this, SLOT(handleGroupsMenu()) );
   }
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
   if( mpDatabase )
   {
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else
   {
      DatabaseInterface::get()->updateTrackInfo( mTrackInfo );
      DatabaseInterface::get()->getTrackInfo( this, "updateTrackInfo", mTrackInfo.mID );
   }
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
   if( mpDatabase )
   {
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else
   {
      DatabaseInterface::get()->updateTrackInfo( mTrackInfo );
      DatabaseInterface::get()->getTrackInfo( this, "updateTrackInfo", mTrackInfo.mID );
   }
}


void TrackInfoWidget::handleGroupsMenu()
{
   if( mpDatabase )
   {
      handleGroupsMenu( mpDatabase->getFolders() );
   }
   else
   {
      DatabaseInterface::get()->getFolders( this, "handleGroupsMenu" );
   }
}


void TrackInfoWidget::handleGroupsMenu( const QStringList &groups )
{
   QMenu menu;
   foreach( const QString &group, groups )
   {
      QAction *action = menu.addAction( group );
      action->setCheckable( true );
      action->setChecked( mTrackInfo.isInFolder( group ) );
   }
   QAction *selected = menu.exec( QCursor::pos() );
   if( selected )
   {
      mTrackInfo.setFolder( selected->text(), selected->isChecked() );
      if( mpDatabase )
      {
         mpDatabase->updateTrackInfo( &mTrackInfo );
      }
      else
      {
         DatabaseInterface::get()->updateTrackInfo( mTrackInfo );
         DatabaseInterface::get()->getTrackInfo( this, "updateTrackInfo", mTrackInfo.mID );
      }
   }
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
   if( mpDatabase )
   {
      mpDatabase->updateTrackInfo( &mTrackInfo );
   }
   else
   {
      DatabaseInterface::get()->updateTrackInfo( mTrackInfo );
      DatabaseInterface::get()->getTrackInfo( this, "updateTrackInfo", mTrackInfo.mID );
   }
}


void TrackInfoWidget::getTrack( const TrackInfo &trackInfo )
{
   mTrackInfo = trackInfo;
   update( false );
}


void TrackInfoWidget::update( bool reread )
{
   if( mpDatabase )
   {
      if( reread )
      {
         mpDatabase->getTrackInfo( &mTrackInfo );
      }
      updateTrackInfo( mTrackInfo );
   }
   else
   {
      if( reread )
      {
         DatabaseInterface::get()->getTrackInfo( this, "updateTrackInfo", mTrackInfo.mID );
      }
      else
      {
         updateTrackInfo( mTrackInfo );
      }
   }
}


void TrackInfoWidget::updateTrackInfo( const TrackInfo &trackInfo )
{
   mTrackInfo = trackInfo;

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
      mpTimesPlayed->setText( tr("%n Time(s) Played", 0, mTrackInfo.mTimesPlayed) );

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
