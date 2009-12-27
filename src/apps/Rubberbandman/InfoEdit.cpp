/**
 * InfoEdit.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QtGui>

#include <fileref.h>
#include <tag.h>

#include "InfoEdit.hpp"
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#include "Database.hpp"
#include "ScrollLine.hpp"
#include "Trace.hpp"

#define MODE_NOTHING     0
#define MODE_SETTAGS     1
#define MODE_NORM_ARTIST 2
#define MODE_NORM_TITLE  3


InfoEdit::InfoEdit( Database *database, QWidget *parent )
: QWidget( parent )
, mpDatabase( database )
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
, mpEditArtist( new QLineEdit( this ) )
, mpEditTitle( new QLineEdit( this ) )
, mpEditAlbum( new QLineEdit( this ) )
, mpEditTrackNr( new QLineEdit( this ) )
, mpEditYear( new QLineEdit( this ) )
, mpEditGenre( new QLineEdit( this ) )
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
, mRecurseMode( 0 )
, mIsValid( false )
, mIsFile( false )
, mCancel( false )
, mTagList()
, mFileName()
, mRecurseArtist()
, mRecurseTitle()
, mRecurseAlbum()
, mRecurseYear()
, mRecurseGenre()
, mRecurseSetFlags( false )
, mRecurseUnsetFlags( false )
, mRecurseFavoriteTrackFlag( false )
, mRecurseUnwantedTrackFlag( false )
, mRecurseTrackScannedFlag( false )
, mRecurseSetFolders( false )
, mRecurseUnsetFolders( false )
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
   
#if QT_VERSION < 0x040300
   fileLayout->setMargin( 2 );
#else
   fileLayout->setVerticalSpacing( 0 );
   fileLayout->setContentsMargins( 2, 2, 2, 2 );
#endif
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

#if QT_VERSION < 0x040300
   tagLayout->setMargin( 2 );
#else
   tagLayout->setVerticalSpacing( 0 );
   tagLayout->setContentsMargins( 2, 2, 2, 2 );
#endif
   mpTagGroupBox->setLayout( tagLayout );
   
   mpDatabaseGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *databaseLayout = new QGridLayout;
   databaseLayout->addWidget( mpButtonFlags,     0, 0 );
   databaseLayout->addWidget( mpShowTimesPlayed, 0, 1 );
   databaseLayout->addWidget( mpButtonFolders,   0, 2 );
   
#if QT_VERSION < 0x040300
   databaseLayout->setMargin( 2 );
#else
   databaseLayout->setVerticalSpacing( 0 );
   databaseLayout->setContentsMargins( 2, 2, 2, 2 );
#endif
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
#if QT_VERSION < 0x040300
   vlayout->setMargin( 2 );
#else
   vlayout->setContentsMargins( 2, 2, 2, 2 );
#endif
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
}


void InfoEdit::recurse( const QDir &dir, bool isBase )
{
   QFileInfoList files(dir.entryInfoList());
   QString fileName;
   int i;
   
   if( isBase )
   {
      if( (mRecurseMode == MODE_SETTAGS) && mRecurseArtist.isEmpty() &&
          mRecurseTitle.isEmpty() && mRecurseAlbum.isEmpty() && 
          mRecurseYear.isEmpty() && mRecurseGenre.isEmpty() && 
          !mpRecurseSetFlags->isChecked() && !mpRecurseUnsetFlags->isChecked() && 
          !mpRecurseSetFolders->isChecked() && !mpRecurseUnsetFolders->isChecked() )
      {
         return;
      }
      
      fileName = mFileName;
      mpButtonSet->setDisabled( true );
      mpButtonNormArtist->setDisabled( true );
      mpButtonNormTitle->setDisabled( true );
      mpButtonCancel->setDisabled( false );
      mCancel = false;
   }
 
   mpDatabase->beginTransaction();
   for( i = 0; i < files.size(); i++ )
   {
      if( mCancel )
      {
         break;
      }
      if( files.at(i).fileName().left(1) == "." )
      {
         continue;
      }
      if( files.at(i).isDir() )
      {
         recurse( QDir( files.at(i).absoluteFilePath() ), false );
      }
      if( files.at(i).isFile() )
      {
         switch( mRecurseMode )
         {
            case MODE_SETTAGS:
               loadFile( files.at(i).absoluteFilePath() );
               if( !mRecurseArtist.isEmpty() )
               {
                  mpEditArtist->setText( mRecurseArtist );
               }
               if( !mRecurseTitle.isEmpty() )
               {
                  mpEditTitle->setText( mpEditTitle->text() + " " + mRecurseTitle );
               }
               if( !mRecurseAlbum.isEmpty() )
               {
                  mpEditAlbum->setText( mRecurseAlbum );
               }
               if( !mRecurseYear.isEmpty() )
               {
                  mpEditYear->setText( mRecurseYear );
               }
               if( !mRecurseGenre.isEmpty() )
               {
                  mpEditGenre->setText( mRecurseGenre );
               }
               if( mRecurseSetFlags )
               {
                  if( mRecurseFavoriteTrackFlag )
                  {
                     mTrackInfo.setFlag( TrackInfo::Favorite, true );
                  }
                  if( mRecurseUnwantedTrackFlag )
                  {
                     mTrackInfo.setFlag( TrackInfo::Unwanted, true );
                  }
               }
               if( mRecurseUnsetFlags )
               {
                  if( mRecurseFavoriteTrackFlag && mTrackInfo.isFlagged( TrackInfo::Favorite ) )
                  {
                     mTrackInfo.setFlag( TrackInfo::Favorite, false );
                  }
                  if( mRecurseUnwantedTrackFlag && mTrackInfo.isFlagged( TrackInfo::Unwanted ) )
                  {
                     mTrackInfo.setFlag( TrackInfo::Unwanted, false );
                  }
                  if( mRecurseTrackScannedFlag )
                  {
                     mTrackInfo.setFlag( TrackInfo::ScannedWithPower, false );
                  }
               }
               if( mRecurseSetFolders || mRecurseUnsetFolders )
               {
                  for( int n = 0; n < mRecurseFolders.size(); n++ )
                  {
                     mTrackInfo.setFolder( mRecurseFolders.at(n), mRecurseSetFolders );
                  }
               }
               QCoreApplication::processEvents();
               saveFile();
               break;
            case MODE_NORM_ARTIST:
               loadFile( files.at(i).absoluteFilePath() );
               normalize( mpEditArtist );
               QCoreApplication::processEvents();
               saveFile();
               break;
            case MODE_NORM_TITLE:
               loadFile( files.at(i).absoluteFilePath() );
               normalize( mpEditTitle );
               QCoreApplication::processEvents();
               saveFile();
               break;
            default:
               break;
         }
      }
   }
   mpDatabase->endTransaction( true );
   
   if( isBase )
   {
      MySettings settings;
      
      mpButtonSet->setDisabled( true );
      mpButtonNormArtist->setDisabled( true );
      mpButtonNormTitle->setDisabled( true );
      mpButtonCancel->setDisabled( true );
         
      mpShowPathName->clear();
      mpShowFileName->clear();
      mpShowSize->clear();
      mpShowPlayTime->clear();
      mpEditArtist->clear();
      mpEditTitle->clear();
      mpEditAlbum->clear();
      mpEditTrackNr->clear();
      mpEditYear->clear();
      mpEditGenre->clear();
      
      mTrackInfo.clear();
      
      settings.sendNotification( QString("r0u") );
      if( settings.VALUE_AUTORESCAN )
      {
         emit updated();
      }
      loadFile( fileName );
   }
}


void InfoEdit::handleNormalizeArtist()
{
   if( mIsFile )
   {
      normalize( mpEditArtist );
   }
   else
   {
      mRecurseMode = MODE_NORM_ARTIST;
      recurse( mFileName );
      mRecurseMode = MODE_NOTHING;
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
      mRecurseMode = MODE_NORM_TITLE;
      recurse( mFileName );
      mRecurseMode = MODE_NOTHING;
   }
}


void InfoEdit::normalize( QLineEdit *lineEdit )
{
   lineEdit->setText( TagList::normalizeString( lineEdit->text() ) );
}


void InfoEdit::load( const QString &fullpath )
{
   if( mRecurseMode == MODE_NOTHING )
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
}


void InfoEdit::loadFile( const QString &fullpath )
{
   mIsValid  = false;
   mIsFile   = false;
   mFileName = fullpath;
   
   QString empty;
   
   QFileInfo fileInfo( fullpath );
   
   mpButtonFlags->setDisabled( false );
   mpButtonFolders->setDisabled( false );
   mpShowTimesPlayed->clear();
   if( fileInfo.isFile() )
   {
      mpButtonNormArtist->setDisabled( false );
      mpButtonNormTitle->setDisabled( false );
      mIsValid = true;
      mIsFile  = true;
      
      mpDatabase->getTrackInfo( &mTrackInfo, fullpath );
      updateDatabaseInfo( false );
      
      mTagList.clear();
      TagLib::FileRef f( fullpath.toLocal8Bit().data() );
      if( f.file() )
      {
         mpShowPathName->setText( fileInfo.absolutePath() );
         mpShowFileName->setText( fileInfo.fileName() );
         mpShowSize->setText( QString::number( fileInfo.size() ) );
         if( f.audioProperties() )
         {
            int length = f.audioProperties()->length();
            QString time( QString::number( length / 60 ) );
            if( (length % 60) < 10 )
            {
               time.append( ":0" );
            }
            else
            {
               time.append( ":" );
            }
            time.append( QString::number( length % 60 ) );
            mpShowPlayTime->setText( time );
         }
         
         QString artist = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         QString title  = QString::fromUtf8( f.tag()->title().toCString( true ) );
         QString album  = QString::fromUtf8( f.tag()->album().toCString( true ) );
         int tracknr    = f.tag()->track();
         int year       = f.tag()->year();
         QString genre  = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         
         mpEditArtist->setText( artist );
         mpEditTitle->setText( title );
         mpEditAlbum->setText( album );
         if( tracknr < 0 )
         {
            mpEditTrackNr->clear();
         }
         else
         {
            mpEditTrackNr->setText( QString::number(tracknr) );
         }
         if( year < 0 )
         {
            mpEditYear->clear();
         }
         else
         {
            mpEditYear->setText( QString::number(year) );
         }
         mpEditGenre->setText( genre );
         
         mTagList.set( "ARTIST", artist );
         mTagList.set( "TITLE", title );
         mTagList.set( "ALBUM", album );
         mTagList.set( "TRACKNUMBER", (tracknr < 0) ? QString() : QString::number(tracknr) );
         mTagList.set( "DATE", (year < 0) ? QString() : QString::number(year) );
         mTagList.set( "GENRE", genre );
      }
      else
      {
         mFileName.clear();
      }
   }
   else
   {
      mTrackInfo.clear();
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
         mpShowPathName->setText( fullpath );
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
   
   if( mRecurseMode == MODE_NOTHING )
   {
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
}

void InfoEdit::handleSetSave()
{
   if( mIsValid )
   {
      if( mIsFile )
      {
         saveFile();
      }
      else
      {
         mRecurseArtist            = mpEditArtist->text();
         mRecurseTitle             = mpEditTitle->text();
         mRecurseAlbum             = mpEditAlbum->text();
         mRecurseYear              = mpEditYear->text();
         mRecurseGenre             = mpEditGenre->text();
         mRecurseFavoriteTrackFlag = mpFavoriteTrackFlag->isChecked();
         mRecurseUnwantedTrackFlag = mpUnwantedTrackFlag->isChecked();
         mRecurseTrackScannedFlag  = mpTrackScannedFlag->isChecked();
         mRecurseFolders           = mTrackInfo.getFolders();
         
         mRecurseMode = MODE_SETTAGS;
         recurse( mFileName );
         mRecurseMode = MODE_NOTHING;
      }
      mpButtonSet->setDisabled( true );
      MySettings().sendNotification( QString("r0u") );
   }
}


void InfoEdit::saveFile()
{
   if( mFileName.isEmpty() ) return;
   if( mpEditArtist->text().isEmpty() ) return;
   if( mpEditTitle->text().isEmpty() ) return;
   
   QFile qf( mFileName );
   QFileInfo qfi( qf );
   QString newname;
   bool tagsChanged = false;
   
   if( mpEditArtist->text() != mTagList.get("ARTIST") )
   {
      mTagList.set( "ARTIST", mpEditArtist->text() );
      tagsChanged = true;
   }
   if( mpEditTitle->text() != mTagList.get("TITLE") )
   {
      mTagList.set( "TITLE", mpEditTitle->text() );
      tagsChanged = true;
   }
   if( mpEditAlbum->text() != mTagList.get("ALBUM") )
   {
      mTagList.set( "ALBUM", mpEditAlbum->text() );
      tagsChanged = true;
   }
   if( mpEditTrackNr->text() != mTagList.get("TRACKNUMBER") )
   {
      mTagList.set( "TRACKNUMBER", mpEditTrackNr->text() );
      tagsChanged = true;
   }
   if( mpEditYear->text() != mTagList.get("DATE") )
   {
      mTagList.set( "DATE", mpEditYear->text() );
      tagsChanged = true;
   }
   if( mpEditGenre->text() != mTagList.get("GENRE") )
   {
      mTagList.set( "GENRE", mpEditGenre->text() );
      tagsChanged = true;
   }
   
   if( mpEditTrackNr->text().isEmpty() )
   {
      newname = mTagList.fileName( MySettings().VALUE_WITHOUTTRACKNR );
   }
   else
   {
      newname = mTagList.fileName( MySettings().VALUE_WITHTRACKNR );
   }
   
   QString newpath( qfi.absolutePath() + "/" + newname + "." + qfi.suffix().toLower() );
   QString tmppath( qfi.absolutePath() + "/" + newname + ".rbm." + qfi.suffix().toLower() );
   
   if( tagsChanged )
   {
      if( qf.copy( tmppath ) )
      {
         bool convertOk;
         TagLib::String artist( mpEditArtist->text().toUtf8().data(), TagLib::String::UTF8 );
         TagLib::String title( mpEditTitle->text().toUtf8().data(), TagLib::String::UTF8 );
         TagLib::String album( mpEditAlbum->text().toUtf8().data(), TagLib::String::UTF8 );
         int tracknr = mpEditTrackNr->text().toInt( &convertOk );
         if( !convertOk )
         {
            tracknr = -1;
         }
         int year    = mpEditYear->text().toInt( &convertOk );
         if( !convertOk )
         {
            year = -1;
         }
         TagLib::String genre( mpEditGenre->text().toUtf8().data(), TagLib::String::UTF8 );
         
         TagLib::FileRef f( tmppath.toLocal8Bit().data() );
         f.tag()->setArtist( artist );
         f.tag()->setTitle( title );
         f.tag()->setAlbum( album );
         f.tag()->setTrack( tracknr );
         f.tag()->setYear( year );
         f.tag()->setGenre( genre );
         f.save();
         QFile( mFileName ).remove();
         QFile::rename( tmppath, newpath );
      }
   }
   else
   {
      QFile::rename( mFileName, newpath );
   }
   
   qfi.setFile( newpath );
   
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
   mTrackInfo.mYear      = mpEditYear->text().toUInt();
   mTrackInfo.mGenre     = mpEditGenre->text();
   
   if( mRecurseMode == MODE_NOTHING )
   {
      mTrackInfo.setFlag( TrackInfo::Favorite, false );
      if( mpFavoriteTrackFlag->isChecked() )
      {
         mTrackInfo.setFlag( TrackInfo::Favorite, true );
      }
      if( mpUnwantedTrackFlag->isChecked() )
      {
         mTrackInfo.setFlag( TrackInfo::Unwanted, true );
      }
      if( !mpTrackScannedFlag->isChecked() )
      {
         mTrackInfo.setFlag( TrackInfo::ScannedWithPeak, false );
      }
   }
   mpDatabase->updateTrackInfo( &mTrackInfo, true );
   
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
   return mTagList.fileName( pattern, filterPath );
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
   int i;
   
   if( (mTrackInfo.mID > 0) || withRecurse )
   {
      QString timesPlayed;
      
      mpMenuFlags->clear();
      mpMenuFolders->clear();
      QStringList folders( mpDatabase->getFolders() );
      
      if( withRecurse )
      {
         mRecurseSetFlags    = true;
         mpRecurseSetFlags   = mpMenuFlags->addAction( tr("Set Selected Flags") );
         mpRecurseSetFlags->setCheckable( true );
         mpRecurseSetFlags->setChecked( mRecurseSetFlags );
         
         mRecurseUnsetFlags  = false;
         mpRecurseUnsetFlags = mpMenuFlags->addAction( tr("Unset Selected Flags") );
         mpRecurseUnsetFlags->setCheckable( true );
         mpRecurseUnsetFlags->setChecked( mRecurseUnsetFlags );
         
         mpMenuFlags->addSeparator();
         
         mRecurseSetFolders    = true;
         mpRecurseSetFolders   = mpMenuFolders->addAction( tr("Set Selected Folders") );
         mpRecurseSetFolders->setCheckable( true );
         mpRecurseSetFolders->setChecked( mRecurseSetFolders );
         
         mRecurseUnsetFolders  = false;
         mpRecurseUnsetFolders = mpMenuFolders->addAction( tr("Unset Selected Folders") );
         mpRecurseUnsetFolders->setCheckable( true );
         mpRecurseUnsetFolders->setChecked( mRecurseUnsetFolders );
         
         mpMenuFolders->addSeparator();
      }
      else
      {
         timesPlayed = QString::number(mTrackInfo.mTimesPlayed);
         if( mTrackInfo.mTimesPlayed == 1 )
         {
            timesPlayed.append( QString(tr(" Time Played")) );
         }
         else
         {
            timesPlayed.append( QString(tr(" Times Played")) );
         }
      }
      mpShowTimesPlayed->setText( timesPlayed );
      
      if( folders.size() > 0 )
      {
         mpButtonFolders->setDisabled( false );
         for( i = 0; i < folders.size(); i++ )
         {
            QAction *action = mpMenuFolders->addAction( folders.at(i) );
            action->setCheckable( true );
            action->setChecked( mTrackInfo.isInFolder( action->text() ) );
         }
      }
      else
      {
         mpButtonFolders->setDisabled( true );
      }
      mpButtonFlags->setDisabled( false );
   }
   else
   {
      mpButtonFlags->setDisabled( true );
      mpButtonFolders->setDisabled( true );
      mpShowTimesPlayed->setText( tr("Not In Database") );
   }
   
   mpFavoriteTrackFlag = mpMenuFlags->addAction( tr("Favorite Track") );
   mpFavoriteTrackFlag->setCheckable( true );
   if( mTrackInfo.isFlagged( TrackInfo::Favorite ) )
   {
      mpFavoriteTrackFlag->setChecked( true );
   }
   mpUnwantedTrackFlag = mpMenuFlags->addAction( tr("Unwanted Track") );
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


void InfoEdit::handleFlagsMenu( QAction *action )
{
   if( action == mpRecurseSetFlags )
   {
      mRecurseSetFlags   = true;
      mRecurseUnsetFlags = false;
      mpRecurseSetFlags->setChecked( mRecurseSetFlags );
      mpRecurseUnsetFlags->setChecked( mRecurseUnsetFlags );
      mpTrackScannedFlag->setChecked( false );
      return;
   }
   
   if( action == mpRecurseUnsetFlags )
   {
      mRecurseSetFlags   = false;
      mRecurseUnsetFlags = true;
      mpRecurseSetFlags->setChecked( mRecurseSetFlags );
      mpRecurseUnsetFlags->setChecked( mRecurseUnsetFlags );
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
          !mRecurseUnsetFlags )
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
      mRecurseSetFolders   = true;
      mRecurseUnsetFolders = false;
      mpRecurseSetFolders->setChecked( mRecurseSetFolders );
      mpRecurseUnsetFolders->setChecked( mRecurseUnsetFolders );
      return;
   }
   
   if( action == mpRecurseUnsetFolders )
   {
      mRecurseSetFolders   = false;
      mRecurseUnsetFolders = true;
      mpRecurseSetFolders->setChecked( mRecurseSetFolders );
      mpRecurseUnsetFolders->setChecked( mRecurseUnsetFolders );
      return;
   }
   
   mTrackInfo.setFolder( action->text(), action->isChecked() );
   handleChange();
}
