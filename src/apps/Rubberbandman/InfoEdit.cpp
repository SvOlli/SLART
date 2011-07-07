/**
 * src/apps/Rubberbandman/InfoEdit.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "InfoEdit.hpp"

/* system headers */
#include <fileref.h>
#include <tag.h>

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <DatabaseInterface.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "LineEdit.hpp"
#include "TrackReader.hpp"
#include "TrackWriter.hpp"
#include "RecurseWorker.hpp"


InfoEdit::InfoEdit( QWidget *parent )
: QWidget( parent )
, mpDatabase( DatabaseInterface::get() )
, mpRecurseWorker( new RecurseWorker() )
, mTrackInfo()
, mpButtonSet( new QPushButton( this ) )
, mpButtonNormArtist( new QPushButton( tr("Norm. Artist"), this ) )
, mpButtonNormTitle( new QPushButton( tr("Norm. Title"), this ) )
, mpButtonCancel( new QPushButton( tr("Cancel"), this ) )
, mpFileGroupBox( new QGroupBox( tr("File Information"), this ) )
, mpTagGroupBox( new QGroupBox( tr("Tag Information"), this ) )
, mpDatabaseGroupBox( new QGroupBox( tr("Database Information"), this ) )
, mpShowPathName( new ScrollLine( this ) )
, mpShowFileName( new ScrollLine( this ) )
, mpShowSize( new ScrollLine( this, false ) )
, mpShowPlayTime( new ScrollLine( this, false ) )
, mpEditArtist( new LineEdit( this ) )
, mpEditTitle( new LineEdit( this ) )
, mpEditAlbum( new LineEdit( this ) )
, mpEditTrackNr( new QLineEdit( this ) )
, mpEditYear( new QLineEdit( this ) )
, mpEditGenre( new LineEdit( this ) )
, mpButtonFlags( new QPushButton( tr("Flags"), this ) )
, mpMenuFlags( new QMenu( this ) )
, mpShowTimesPlayed( new QLabel( this ) )
, mpButtonFolders( new QPushButton( tr("Folders"), this ) )
, mpMenuFolders( new QMenu( this ) )
, mpRecurseSetFlags( 0 )
, mpRecurseUnsetFlags( 0 )
, mpFavoriteTrackFlag( 0 )
, mpUnwantedTrackFlag( 0 )
, mpTrackScannedFlag( 0 )
, mpRecurseSetFolders( 0 )
, mpRecurseUnsetFolders( 0 )
, mIsValid( false )
, mIsFile( false )
, mCancel( false )
, mFileName()
{
   mpButtonFlags->setMenu( mpMenuFlags );
   mpButtonFolders->setMenu( mpMenuFolders );
   mpShowTimesPlayed->setAlignment( Qt::AlignCenter );

   mpValidateTrackNr = new QIntValidator( 0, 99, mpEditTrackNr );
   mpValidateYear    = new QIntValidator( 0, 9999, mpEditYear );
   mpEditTrackNr->setValidator( mpValidateTrackNr );
   mpEditYear->setValidator( mpValidateYear );
   mpEditTrackNr->setMaxLength( 2 );
   mpEditYear->setMaxLength( 4 );
   mpEditTrackNr->setMaximumWidth( QFontMetrics(mpEditTrackNr->font()).width("999 ") );
   mpEditTrackNr->setMinimumWidth( mpEditTrackNr->maximumWidth() );
   mpEditYear->setMaximumWidth( QFontMetrics(mpEditYear->font()).width("99999 ") );
   mpEditYear->setMinimumWidth( mpEditYear->maximumWidth() );
   mpShowSize->setReadOnly( true );
   mpShowPlayTime->setReadOnly( true );

   mpFileGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *fileLayout = new QGridLayout;

   fileLayout->addWidget( new QLabel( tr("Path:"), this ),      0, 0 );
   fileLayout->addWidget( new QLabel( tr("File:"), this ),      1, 0 );
   fileLayout->addWidget( new QLabel( tr("Size:"), this ),      2, 0 );
   fileLayout->addWidget( new QLabel( tr("Play Time:"), this ), 2, 3 );

   fileLayout->addWidget( mpShowPathName, 0, 1, 1, 4 );
   fileLayout->addWidget( mpShowFileName, 1, 1, 1, 4 );
   fileLayout->addWidget( mpShowSize,     2, 1 );
   fileLayout->addWidget( mpShowPlayTime, 2, 4 );

   fileLayout->setColumnStretch( 2, 1 );

   fileLayout->setVerticalSpacing( 0 );
   fileLayout->setContentsMargins( 2, 2, 2, 2 );
   mpFileGroupBox->setLayout( fileLayout );

   mpTagGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *tagLayout = new QGridLayout;

   tagLayout->addWidget( new QLabel( tr("Artist:"), this ), 0, 0 );
   tagLayout->addWidget( new QLabel( tr("Title:"), this ),  1, 0 );
   tagLayout->addWidget( new QLabel( tr("Album:"), this ),  2, 0 );
   tagLayout->addWidget( new QLabel( tr("Track:"), this ),  3, 0 );
   tagLayout->addWidget( new QLabel( tr("Year:"), this ),   3, 2 );
   tagLayout->addWidget( new QLabel( tr("Genre:"), this ),  3, 4 );

   tagLayout->addWidget( mpEditArtist,  0, 1, 1, 5 );
   tagLayout->addWidget( mpEditTitle,   1, 1, 1, 5 );
   tagLayout->addWidget( mpEditAlbum,   2, 1, 1, 5 );
   tagLayout->addWidget( mpEditTrackNr, 3, 1 );
   tagLayout->addWidget( mpEditYear,    3, 3 );
   tagLayout->addWidget( mpEditGenre,   3, 5 );

   tagLayout->setColumnStretch( 0,  1 );
   tagLayout->setColumnStretch( 1,  2 );
   tagLayout->setColumnStretch( 2,  1 );
   tagLayout->setColumnStretch( 3, 15 );
   tagLayout->setColumnStretch( 4,  1 );
   tagLayout->setColumnStretch( 5, 80 );

   tagLayout->setVerticalSpacing( 0 );
   tagLayout->setContentsMargins( 2, 2, 2, 2 );
   mpTagGroupBox->setLayout( tagLayout );

   mpDatabaseGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *databaseLayout = new QGridLayout;
   databaseLayout->addWidget( mpButtonFlags,     0, 0 );
   databaseLayout->addWidget( mpShowTimesPlayed, 0, 1 );
   databaseLayout->addWidget( mpButtonFolders,   0, 2 );

   databaseLayout->setVerticalSpacing( 0 );
   databaseLayout->setContentsMargins( 2, 2, 2, 2 );
   mpDatabaseGroupBox->setLayout( databaseLayout );

   QHBoxLayout *hlayout = new QHBoxLayout;
   hlayout->addWidget( mpButtonSet );
   hlayout->addWidget( mpButtonNormArtist );
   hlayout->addWidget( mpButtonNormTitle );
   hlayout->addWidget( mpButtonCancel );
   mpButtonSet->setDisabled( true );
   mpButtonNormArtist->setDisabled( true );
   mpButtonNormTitle->setDisabled( true );
   mpButtonCancel->setDisabled( true );

   QVBoxLayout *vlayout = new QVBoxLayout;
   vlayout->addLayout( hlayout );
   vlayout->addWidget( mpFileGroupBox );
   vlayout->addWidget( mpTagGroupBox );
   vlayout->addWidget( mpDatabaseGroupBox );
   vlayout->setContentsMargins( 2, 2, 2, 2 );
   setLayout( vlayout );

   setMaximumHeight( vlayout->minimumSize().height() );
   load(QString());

   connect( mpButtonSet, SIGNAL(clicked()),
            this, SLOT(handleSetSave()) );
   connect( mpButtonNormArtist, SIGNAL(clicked()),
            this, SLOT(handleNormalizeArtist()) );
   connect( mpButtonNormTitle,  SIGNAL(clicked()),
            this, SLOT(handleNormalizeTitle()) );
   connect( mpEditArtist, SIGNAL(textChanged(const QString&)),
            this, SLOT(handleChange()) );
   connect( mpEditTitle, SIGNAL(textChanged(const QString&)),
            this, SLOT(handleChange()) );
   connect( mpEditAlbum, SIGNAL(textChanged(const QString&)),
            this, SLOT(handleChange()) );
   connect( mpEditTrackNr, SIGNAL(textChanged(const QString&)),
            this, SLOT(handleChange()) );
   connect( mpEditYear, SIGNAL(textChanged(const QString&)),
            this, SLOT(handleChange()) );
   connect( mpEditGenre, SIGNAL(textChanged(const QString&)),
            this, SLOT(handleChange()) );

   connect( mpMenuFlags, SIGNAL(triggered(QAction*)),
            this, SLOT(handleFlagsMenu(QAction*)) );
   connect( mpMenuFolders, SIGNAL(triggered(QAction*)),
            this, SLOT(handleFoldersMenu(QAction*)) );

   mpDatabase->getAllColumnData( mpEditArtist, "setCompleterTexts", DatabaseInterface::Artist );
   mpDatabase->getAllColumnData( mpEditTitle,  "setCompleterTexts", DatabaseInterface::Title );
   mpDatabase->getAllColumnData( mpEditAlbum,  "setCompleterTexts", DatabaseInterface::Album );
   mpDatabase->getAllColumnData( mpEditGenre,  "setCompleterTexts", DatabaseInterface::Genre );
}


InfoEdit::~InfoEdit()
{
   delete mpRecurseWorker;
}


void InfoEdit::handleNormalizeArtist()
{
   if( mIsFile )
   {
      normalize( mpEditArtist );
   }
   else
   {
      mpRecurseWorker->startNormArtist( mFileName );
   }
}


void InfoEdit::handleNormalizeTitle()
{
   if( mIsFile )
   {
      normalize( mpEditTitle );
   }
   else
   {
      mpRecurseWorker->startNormTitle( mFileName );
   }
}


void InfoEdit::normalize( QLineEdit *lineEdit )
{
   lineEdit->setText( TagList::normalizeString( lineEdit->text() ) );
}


void InfoEdit::load( const QString &fullpath )
{
   if( fullpath == "-" )
   {
      loadFile( mFileName );
   }
   else
   {
      loadFile( fullpath );
   }
}


void InfoEdit::loadFile( const QString &fullpath )
{
   mFileName = fullpath;
   QFileInfo fileInfo( mFileName );
   if( fileInfo.isFile() )
   {
      TrackReader *tr = new TrackReader( this, "loadTrackInfo" );
      tr->read( mFileName );
   }
   else
   {
      mTrackInfo.clear();
      mTrackInfo.mDirectory = mFileName;
      loadTrackInfo( mTrackInfo );
   }
}


void InfoEdit::loadTrackInfo( const TrackInfo &trackInfo )
{
   mIsValid  = false;
   mIsFile   = false;

   mTrackInfo = trackInfo;
   QFileInfo fileInfo( mFileName );
   if( !fileInfo.exists() && !trackInfo.mDirectory.isEmpty() )
   {
      mFileName  = trackInfo.mDirectory + "/" + trackInfo.mFileName;
      fileInfo.setFile( mFileName );
   }

   mpButtonFlags->setDisabled( false );
   mpButtonFolders->setDisabled( false );
   mpShowTimesPlayed->clear();
   if( fileInfo.isFile() )
   {
      mpButtonNormArtist->setDisabled( false );
      mpButtonNormTitle->setDisabled( false );
      mIsValid = true;
      mIsFile  = true;
      updateDatabaseInfo( false );

      TagList tagList;
      mpShowPathName->setText( fileInfo.absolutePath() );
      mpShowFileName->setText( fileInfo.fileName() );
      mpShowSize->setText( QString::number( fileInfo.size() ) );
      QString playtime( QString::number( trackInfo.mPlayTime / 60 ) );
      if( (trackInfo.mPlayTime % 60) < 10 )
      {
         playtime.append( ":0" );
      }
      else
      {
         playtime.append( ":" );
      }
      playtime.append( QString::number( trackInfo.mPlayTime % 60 ) );
      mpShowPlayTime->setText( playtime );
      mpEditArtist->setText( trackInfo.mArtist );
      mpEditTitle->setText( trackInfo.mTitle );
      mpEditAlbum->setText( trackInfo.mAlbum );
      if( trackInfo.mTrackNr < 0 )
      {
         mpEditTrackNr->clear();
      }
      else
      {
         mpEditTrackNr->setText( QString::number(trackInfo.mTrackNr) );
      }
      if( trackInfo.mYear < 0 )
      {
         mpEditYear->clear();
      }
      else
      {
         mpEditYear->setText( QString::number(trackInfo.mYear) );
      }
      mpEditGenre->setText( trackInfo.mGenre );

      tagList.set( "ARTIST", trackInfo.mArtist );
      tagList.set( "TITLE", trackInfo.mTitle );
      tagList.set( "ALBUM", trackInfo.mAlbum );
      tagList.set( "TRACKNUMBER", (trackInfo.mTrackNr < 0) ? QString() : QString::number(trackInfo.mTrackNr) );
      tagList.set( "DATE", (trackInfo.mYear < 0) ? QString() : QString::number(trackInfo.mYear) );
      tagList.set( "GENRE", trackInfo.mGenre );
      tagList.set( "DIRECTORY", trackInfo.mDirectory );
      tagList.set( "FILENAME", trackInfo.mFileName );
   }
   else
   {
      updateDatabaseInfo( true );
      mpShowFileName->clear();
      mpShowSize->clear();
      mpShowPlayTime->clear();
      mpEditArtist->clear();
      mpEditTitle->clear();
      mpEditAlbum->clear();
      mpEditTrackNr->clear();
      mpEditYear->clear();
      mpEditGenre->clear();

      if( fileInfo.isDir() )
      {
         mpButtonNormArtist->setDisabled( false );
         mpButtonNormTitle->setDisabled( false );
         mIsValid = true;
         mpShowPathName->setText( mFileName );
      }
      else
      {
         mpShowPathName->clear();
         mpButtonNormArtist->setDisabled( true );
         mpButtonNormTitle->setDisabled( true );
         mpButtonFlags->setDisabled( true );
         mpButtonFolders->setDisabled( true );
      }
   }

   if( mIsFile )
   {
      mpButtonSet->setText( tr("Save Changes") );
      mpEditTrackNr->setDisabled( false );
      mpShowFileName->setDisabled( false );
      mpShowSize->setDisabled( false );
      mpShowPlayTime->setDisabled( false );
      mpButtonSet->setDisabled( mTrackInfo.mID > 0 );
   }
   else
   {
      mpButtonSet->setText( tr("Set Recursive") );
      mpEditTrackNr->setDisabled( true );
      mpShowFileName->setDisabled( true );
      mpShowSize->setDisabled( true );
      mpShowPlayTime->setDisabled( true );
      mpButtonSet->setDisabled( true );
   }
}


void InfoEdit::handleSetSave()
{
   if( mIsValid )
   {
      mpEditArtist->addCompleterText();
      mpEditTitle->addCompleterText();
      mpEditAlbum->addCompleterText();
      mpEditGenre->addCompleterText();

      if( mIsFile )
      {
         saveFile();
      }
      else
      {
         RecurseWorker::Flags recurseFlags;
         TrackInfo ti;
         if( mpFavoriteTrackFlag->isChecked() )
         {
            ti.setFlag( TrackInfo::Favorite, true );
         }
         if( mpUnwantedTrackFlag->isChecked() )
         {
            ti.setFlag( TrackInfo::Unwanted, true );
         }
         if( mpTrackScannedFlag->isChecked() )
         {
            ti.setFlag( TrackInfo::ScannedWithPeak, true );
         }
         if( mpRecurseSetFlags->isChecked() )
         {
            recurseFlags |= RecurseWorker::SetFlags;
         }
         if( mpRecurseUnsetFlags->isChecked() )
         {
            recurseFlags |= RecurseWorker::UnsetFlags;
         }
         if( mpRecurseSetFolders->isChecked() )
         {
            recurseFlags |= RecurseWorker::SetGroups;
         }
         if( mpRecurseUnsetFolders->isChecked() )
         {
            recurseFlags |= RecurseWorker::UnsetGroups;
         }
         ti.mArtist  = mpEditArtist->text();
         ti.mTitle   = mpEditTitle->text();
         ti.mAlbum   = mpEditAlbum->text();
         if( mpEditYear->text().isEmpty() )
         {
            ti.mYear = -1;
         }
         else
         {
            ti.mYear = mpEditYear->text().toUInt();
         }
         ti.mGenre   = mpEditGenre->text();
         ti.mFolders = mTrackInfo.mFolders;

         mpRecurseWorker->startSetTags( mFileName, ti, recurseFlags );
      }
      mpButtonSet->setDisabled( true );
   }
}


void InfoEdit::saveFile()
{
   if( mFileName.isEmpty() ) return;
   if( mpEditArtist->text().isEmpty() ) return;
   if( mpEditTitle->text().isEmpty() ) return;

   QFileInfo qfi( mFileName );
   mTrackInfo.mDirectory    = qfi.absolutePath();
   mTrackInfo.mFileName     = qfi.fileName();
   mTrackInfo.mLastTagsRead = qfi.lastModified().toTime_t();
   mTrackInfo.mArtist       = mpEditArtist->text();
   mTrackInfo.mTitle        = mpEditTitle->text();
   mTrackInfo.mAlbum        = mpEditAlbum->text();
   if( mpEditTrackNr->text().isEmpty() )
   {
      mTrackInfo.mTrackNr   = -1;
   }
   else
   {
      mTrackInfo.mTrackNr   = mpEditTrackNr->text().toUInt();
   }
   if( mpEditYear->text().isEmpty() )
   {
      mTrackInfo.mYear      = -1;
   }
   else
   {
      mTrackInfo.mYear      = mpEditYear->text().toUInt();
   }
   mTrackInfo.mGenre        = mpEditGenre->text();

   TrackWriter *tw = new TrackWriter( this, "loadTrackInfo" );
   tw->write( mTrackInfo );

   if( mTrackInfo.mID <= 0 )
   {
      loadFile( mTrackInfo.filePath() );
   }
   else
   {
      mpShowPathName->setText( qfi.absolutePath() );
      mpShowFileName->setText( qfi.fileName() );
      mFileName = qfi.absoluteFilePath();
   }
}


QString InfoEdit::tagsFileName( const QString &pattern, bool filterPath )
{
   TagList tagList;
   tagList.set( "ARTIST", mTrackInfo.mArtist );
   tagList.set( "TITLE", mTrackInfo.mTitle );
   tagList.set( "ALBUM", mTrackInfo.mAlbum );
   tagList.set( "TRACKNUMBER", (mTrackInfo.mTrackNr < 0) ? QString() : QString::number(mTrackInfo.mTrackNr) );
   tagList.set( "DATE", (mTrackInfo.mYear < 0) ? QString() : QString::number(mTrackInfo.mYear) );
   tagList.set( "GENRE", mTrackInfo.mGenre );
   tagList.set( "DIRECTORY", mTrackInfo.mDirectory );
   tagList.set( "FILENAME", mTrackInfo.mFileName );
   return tagList.fileName( pattern, filterPath );
}


QString InfoEdit::fileName()
{
   return mFileName;
}


void InfoEdit::handleCancel()
{
   mCancel = true;
}


void InfoEdit::handleChange()
{
   mpButtonSet->setDisabled( mpShowPathName->text().isEmpty() );
}


void InfoEdit::updateDatabaseInfo( bool withRecurse )
{
   if( (mTrackInfo.mID > 0) || withRecurse )
   {
      QString timesPlayed;

      mpMenuFlags->clear();
      mpMenuFolders->clear();
      mpDatabase->getFolders( this, "handleFoldersEntries" );

      if( withRecurse )
      {
         mpRecurseSetFlags   = mpMenuFlags->addAction( tr("Set Selected Flags") );
         mpRecurseSetFlags->setCheckable( true );
         mpRecurseSetFlags->setChecked( true );

         mpRecurseUnsetFlags = mpMenuFlags->addAction( tr("Unset Selected Flags") );
         mpRecurseUnsetFlags->setCheckable( true );
         mpRecurseUnsetFlags->setChecked( false );

         mpMenuFlags->addSeparator();

         mpRecurseSetFolders   = mpMenuFolders->addAction( tr("Set Selected Folders") );
         mpRecurseSetFolders->setCheckable( true );
         mpRecurseSetFolders->setChecked( true );

         mpRecurseUnsetFolders = mpMenuFolders->addAction( tr("Unset Selected Folders") );
         mpRecurseUnsetFolders->setCheckable( true );
         mpRecurseUnsetFolders->setChecked( false );

         mpMenuFolders->addSeparator();
      }
      else
      {
         timesPlayed = tr("%n Time(s) Played", 0, mTrackInfo.mTimesPlayed);
      }
      mpShowTimesPlayed->setText( timesPlayed );

      mpButtonFlags->setDisabled( false );
   }
   else
   {
      mpButtonFlags->setDisabled( true );
      mpButtonFolders->setDisabled( true );
      mpShowTimesPlayed->setText( tr("Not In Database") );
   }

   mpFavoriteTrackFlag = mpMenuFlags->addAction( tr("Favorite") );
   mpFavoriteTrackFlag->setCheckable( true );
   if( mTrackInfo.isFlagged( TrackInfo::Favorite ) )
   {
      mpFavoriteTrackFlag->setChecked( true );
   }
   mpUnwantedTrackFlag = mpMenuFlags->addAction( tr("No Autoplay") );
   mpUnwantedTrackFlag->setCheckable( true );
   if( mTrackInfo.isFlagged( TrackInfo::Unwanted ) )
   {
      mpUnwantedTrackFlag->setChecked( true );
   }
   mpTrackScannedFlag  = mpMenuFlags->addAction( tr("Track Is Scanned") );
   mpTrackScannedFlag->setCheckable( true );
   if( mTrackInfo.isFlagged( TrackInfo::ScannedWithPeak ) ||
       mTrackInfo.isFlagged( TrackInfo::ScannedWithPower ) )
   {
      mpTrackScannedFlag->setChecked( true );
   }
}


void InfoEdit::handleFoldersEntries( const QStringList &folders )
{
   if( folders.size() > 0 )
   {
      mpButtonFolders->setDisabled( false );
      foreach( const QString &folder, folders )
      {
         QAction *action = mpMenuFolders->addAction( folder );
         action->setCheckable( true );
         action->setChecked( mTrackInfo.isInFolder( action->text() ) );
      }
   }
   else
   {
      mpButtonFolders->setDisabled( true );
   }
}


void InfoEdit::handleFlagsMenu( QAction *action )
{
   if( action == mpRecurseSetFlags )
   {
      mpRecurseUnsetFlags->setChecked( false );
      mpTrackScannedFlag->setChecked( false );
      return;
   }

   if( action == mpRecurseUnsetFlags )
   {
      mpRecurseSetFlags->setChecked( false );
      return;
   }

   if( action == mpFavoriteTrackFlag )
   {
      mpUnwantedTrackFlag->setChecked( false );
   }

   if( action == mpUnwantedTrackFlag )
   {
      mpFavoriteTrackFlag->setChecked( false );
   }

   if( action == mpTrackScannedFlag )
   {
      if( !mTrackInfo.isFlagged( TrackInfo::ScannedWithPeak ) &&
          !mTrackInfo.isFlagged( TrackInfo::ScannedWithPower ) &&
          !mpRecurseUnsetFlags->isChecked() )
      {
         mpTrackScannedFlag->setChecked( false );
      }
   }

   handleChange();
}


void InfoEdit::handleFoldersMenu( QAction *action )
{
   if( action == mpRecurseSetFolders )
   {
      mpRecurseUnsetFolders->setChecked( false );
      return;
   }

   if( action == mpRecurseUnsetFolders )
   {
      mpRecurseSetFolders->setChecked( false );
      return;
   }

   mTrackInfo.setFolder( action->text(), action->isChecked() );
   handleChange();
}
