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
#include "Database.hpp"
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
, mpLabelPathName( new QLabel( tr("Path:"), this ) )
, mpLabelFileName( new QLabel( tr("File:"), this ) )
, mpLabelArtist( new QLabel( tr("Artist:"), this ) )
, mpLabelTitle( new QLabel( tr("Title:"), this ) )
, mpLabelAlbum( new QLabel( tr("Album:"), this ) )
, mpLabelTrackNr( new QLabel( tr("TrackNr:"), this ) )
, mpLabelYear( new QLabel( tr("Year:"), this ) )
, mpLabelGenre( new QLabel( tr("Genre:"), this ) )
, mpShowPathName( new QLineEdit( this ) )
, mpShowFileName( new QLineEdit( this ) )
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
   
#if 1
   mpValidateTrackNr = new QIntValidator( 0, 99, mpEditTrackNr );
   mpValidateYear    = new QIntValidator( 1900, 2100, mpEditYear );
   mpEditTrackNr->setValidator( mpValidateTrackNr );
   mpEditYear->setValidator( mpValidateYear );
#endif
   mpEditTrackNr->setMaxLength( 2 );
   mpEditYear->setMaxLength( 4 );
   mpShowPathName->setReadOnly( true );
   mpShowFileName->setReadOnly( true );
   
#if 0
   mpLabelArtist->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
   mpLabelTitle->setAlignment( Qt::AlignRight | Qt::AlignVCenter  );
   mpLabelAlbum->setAlignment( Qt::AlignRight | Qt::AlignVCenter  );
   mpLabelTrackNr->setAlignment( Qt::AlignRight | Qt::AlignVCenter  );
#endif

   mpFileGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *fileLayout = new QGridLayout;
   
   fileLayout->addWidget( mpLabelPathName, 0, 0 );
   fileLayout->addWidget( mpLabelFileName, 1, 0 );
   
   fileLayout->addWidget( mpShowPathName,  0, 1, 1, 5 );
   fileLayout->addWidget( mpShowFileName,  1, 1, 1, 5 );
   
   fileLayout->setColumnStretch( 0,  1 );
   fileLayout->setColumnStretch( 1, 99 );
   
#if QT_VERSION < 0x040300
   fileLayout->setMargin( 2 );
#else
   fileLayout->setVerticalSpacing( 0 );
   fileLayout->setContentsMargins( 2, 2, 2, 2 );
#endif
   mpFileGroupBox->setLayout( fileLayout );
   
   mpTagGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *tagLayout = new QGridLayout;
   
   tagLayout->addWidget( mpLabelArtist,   0, 0 );
   tagLayout->addWidget( mpLabelTitle,    1, 0 );
   tagLayout->addWidget( mpLabelAlbum,    2, 0 );
   tagLayout->addWidget( mpLabelTrackNr,  3, 0 );
   tagLayout->addWidget( mpLabelYear,     3, 2 );
   tagLayout->addWidget( mpLabelGenre,    3, 4 );
   
   tagLayout->addWidget( mpEditArtist,    0, 1, 1, 5 );
   tagLayout->addWidget( mpEditTitle,     1, 1, 1, 5 );
   tagLayout->addWidget( mpEditAlbum,     2, 1, 1, 5 );
   tagLayout->addWidget( mpEditTrackNr,   3, 1 );
   tagLayout->addWidget( mpEditYear,      3, 3 );
   tagLayout->addWidget( mpEditGenre,     3, 5 );

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
TRACESTART(InfoEdit::recurse)
   QFileInfoList files(dir.entryInfoList());
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
               load( files.at(i).absoluteFilePath() );
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
TRACEMSG << "SetFlags" << mRecurseFavoriteTrackFlag << mRecurseUnwantedTrackFlag;
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
TRACEMSG << "UnsetFlags" << mRecurseFavoriteTrackFlag << mRecurseUnwantedTrackFlag << mRecurseTrackScannedFlag;
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
TRACEMSG << mTrackInfo.toString();
               saveFile();
               break;
            case MODE_NORM_ARTIST:
               load( files.at(i).absoluteFilePath() );
               normalize( mpEditArtist );
               QCoreApplication::processEvents();
               saveFile();
               break;
            case MODE_NORM_TITLE:
               load( files.at(i).absoluteFilePath() );
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
      mpButtonSet->setDisabled( false );
      mpButtonNormArtist->setDisabled( false );
      mpButtonNormTitle->setDisabled( false );
      mpButtonCancel->setDisabled( true );
         
      mpShowPathName->clear();
      mpShowFileName->clear();
      mpEditArtist->clear();
      mpEditTitle->clear();
      mpEditAlbum->clear();
      mpEditTrackNr->clear();
      mpEditYear->clear();
      mpEditGenre->clear();
      
      mTrackInfo.clear();
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
#if 0
TRACESTART(InfoEdit::load)
TRACEMSG << fullpath;
#endif
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
      mIsValid = true;
      mIsFile  = true;
      
      mpDatabase->getTrackInfo( &mTrackInfo, fullpath );
      
      mpShowTimesPlayed->setText( QString::number(mTrackInfo.mTimesPlayed) + " Times Played" );
      updateMenus( false );
      
      mTagList.clear();
      TagLib::FileRef f( fullpath.toLocal8Bit().data() );
      if( f.file() )
      {
         int fileNameStart = fullpath.lastIndexOf('/')+1;
         mpShowPathName->setText( fullpath.left(fileNameStart) );
         mpShowFileName->setText( fullpath.mid(fileNameStart) );
         
         QString artist = QString::fromUtf8( f.tag()->artist().toCString( true ) );
         QString title  = QString::fromUtf8( f.tag()->title().toCString( true ) );
         QString album  = QString::fromUtf8( f.tag()->album().toCString( true ) );
         uint tracknr   = f.tag()->track();
         uint year      = f.tag()->year();
         QString genre  = QString::fromUtf8( f.tag()->genre().toCString( true ) );
         
         mpEditArtist->setText( artist );
         mpEditTitle->setText( title );
         mpEditAlbum->setText( album );
         mpEditTrackNr->setText( QString::number(tracknr) );
         mpEditYear->setText( QString::number(year) );
         mpEditGenre->setText( genre );
         
         mTagList.set( "ARTIST", artist );
         mTagList.set( "TITLE", title );
         mTagList.set( "ALBUM", album );
         mTagList.set( "TRACKNUMBER", QString::number(tracknr) );
         mTagList.set( "DATE", QString::number(year) );
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
      updateMenus( true );
      mpShowFileName->clear();
      mpEditArtist->clear();
      mpEditTitle->clear();
      mpEditAlbum->clear();
      mpEditTrackNr->clear();
      mpEditYear->clear();
      mpEditGenre->clear();
      
      if( fileInfo.isDir() )
      {
         mIsValid = true;
         mpShowPathName->setText( fullpath );
      }
      else
      {
         mpShowPathName->clear();
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
      }
      else
      {
         mpButtonSet->setText( tr("Set Recursive") );
         mpEditTrackNr->setDisabled( true );
         mpShowFileName->setDisabled( true );
      }
   }
   
   mpButtonSet->setDisabled( true );
#if 0
TRACEMSG << "mIsValid" << mIsValid << "mIsFile" << mIsFile;
#endif
}

void InfoEdit::handleSetSave()
{
#if 0
TRACESTART(InfoEdit::handleSetSave)
TRACEMSG << mIsValid << mIsFile;
#endif
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
   }
}


void InfoEdit::saveFile()
{
#if 0
TRACESTART(InfoEdit::saveFile)
TRACEMSG << mFileName;
#endif
   
   if( mFileName.isEmpty() ) return;
   if( mpEditArtist->text().isEmpty() ) return;
   if( mpEditTitle->text().isEmpty() ) return;
   
   QFile qf( mFileName );
   QFileInfo qfi( qf );
   QString newname;
   
   mTagList.set( "ARTIST",      mpEditArtist->text() );
   mTagList.set( "TITLE",       mpEditTitle->text() );
   mTagList.set( "ALBUM",       mpEditAlbum->text() );
   mTagList.set( "TRACKNUMBER", mpEditTrackNr->text() );
   mTagList.set( "DATE",        mpEditYear->text() );
   mTagList.set( "GENRE",       mpEditGenre->text() );
   
   if( mpEditTrackNr->text().isEmpty() )
   {
      newname = mTagList.fileName( MySettings().value("WithoutTrackNr", "|$ARTIST| - |$TITLE|").toString() );
   }
   else
   {
      newname = mTagList.fileName( MySettings().value("WithTrackNr", "(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|").toString() );
   }
   
   QString newpath( qfi.absolutePath() + "/" + newname + "." + qfi.suffix().toLower() );
   QString tmppath( qfi.absolutePath() + "/" + newname + ".rbm." + qfi.suffix().toLower() );
   
   if( qf.copy( tmppath ) )
   {
      TagLib::String artist( mpEditArtist->text().toUtf8().data(), TagLib::String::UTF8 );
      TagLib::String title( mpEditTitle->text().toUtf8().data(), TagLib::String::UTF8 );
      TagLib::String album( mpEditAlbum->text().toUtf8().data(), TagLib::String::UTF8 );
      int tracknr = mpEditTrackNr->text().toInt();
      int year    = mpEditYear->text().toInt();
      TagLib::String genre( mpEditGenre->text().toUtf8().data(), TagLib::String::UTF8 );
      
      TagLib::FileRef f( tmppath.toLocal8Bit().data() );
      f.tag()->setArtist( artist );
      f.tag()->setTitle( title );
      f.tag()->setAlbum( album );
      f.tag()->setTrack( tracknr );
      f.tag()->setYear( year );
      f.tag()->setGenre( genre );
      f.save();
      ::unlink( mFileName.toLocal8Bit().data() );
      ::rename( tmppath.toLocal8Bit().data(), newpath.toLocal8Bit().data() );
   }
   
   qfi.setFile( newpath );
   
   mTrackInfo.mDirectory = qfi.absolutePath();
   mTrackInfo.mFileName  = qfi.fileName();
   mTrackInfo.mArtist    = mpEditArtist->text();
   mTrackInfo.mTitle     = mpEditTitle->text();
   mTrackInfo.mAlbum     = mpEditAlbum->text();
   mTrackInfo.mTrackNr   = mpEditTrackNr->text().toUInt();
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
   mpDatabase->updateTrackInfo( &mTrackInfo );
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
   mpButtonSet->setDisabled( false );
}


void InfoEdit::updateMenus( bool withRecurse )
{
   int i;
   
   mpMenuFlags->clear();
   mpMenuFolders->clear();
   QStringList folders( mpDatabase->getFolders() );
   
   if( withRecurse )
   {
      mRecurseSetFlags    = false;
      mpRecurseSetFlags   = mpMenuFlags->addAction( QString( tr("Set Selected Flags") ) );
      mpRecurseSetFlags->setCheckable( true );
      
      mRecurseUnsetFlags  = false;
      mpRecurseUnsetFlags = mpMenuFlags->addAction( QString( tr("Unset Selected Flags") ) );
      mpRecurseUnsetFlags->setCheckable( true );
      
      mpMenuFlags->addSeparator();
      
      mRecurseSetFolders    = false;
      mpRecurseSetFolders   = mpMenuFolders->addAction( QString( tr("Set Selected Folders") ) );
      mpRecurseSetFolders->setCheckable( true );
      
      mRecurseUnsetFolders  = false;
      mpRecurseUnsetFolders = mpMenuFolders->addAction( QString( tr("Unset Selected Folders") ) );
      mpRecurseUnsetFolders->setCheckable( true );
      
      mpMenuFolders->addSeparator();
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
   
   for( i = 0; i < folders.size(); i++ )
   {
      QAction *action = mpMenuFolders->addAction( folders.at(i) );
      action->setCheckable( true );
      action->setChecked( mTrackInfo.isInFolder( action->text() ) );
   }
}


void InfoEdit::handleFlagsMenu( QAction *action )
{
   if( action == mpRecurseSetFlags )
   {
      mRecurseSetFlags   = action->isChecked();
      mRecurseUnsetFlags = false;
      mpRecurseUnsetFlags->setChecked( false );
      mpTrackScannedFlag->setChecked( false );
      return;
   }
   
   if( action == mpRecurseUnsetFlags )
   {
      mRecurseUnsetFlags = action->isChecked();
      mRecurseSetFlags   = false;
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
      mRecurseSetFolders = action->isChecked();
      mpRecurseUnsetFolders->setChecked( false );
      return;
   }
   
   if( action == mpRecurseUnsetFolders )
   {
      mRecurseUnsetFolders = action->isChecked();
      mpRecurseSetFolders->setChecked( false );
      return;
   }
   
   mTrackInfo.setFolder( action->text(), action->isChecked() );
   handleChange();
}
