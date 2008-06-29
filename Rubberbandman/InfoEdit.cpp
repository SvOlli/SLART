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
, mpSelectFlavor( new QComboBox( this ) )
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
, mRecurseFlavor( 3 )
{
   mpSelectFlavor->addItem( QString(tr("Normal Track")) );
   mpSelectFlavor->addItem( QString(tr("Favorite Track")) );
   mpSelectFlavor->addItem( QString(tr("Unwanted Track")) );
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
   fileLayout->setContentsMargins( 2, 2, 2, 2 );
#endif
   mpFileGroupBox->setLayout( fileLayout );
   
   mpTagGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *netLayout = new QGridLayout;
   
   netLayout->addWidget( mpLabelArtist,   0, 0 );
   netLayout->addWidget( mpLabelTitle,    1, 0 );
   netLayout->addWidget( mpLabelAlbum,    2, 0 );
   netLayout->addWidget( mpLabelTrackNr,  3, 0 );
   netLayout->addWidget( mpLabelYear,     3, 2 );
   netLayout->addWidget( mpLabelGenre,    3, 4 );
   
   netLayout->addWidget( mpEditArtist,    0, 1, 1, 5 );
   netLayout->addWidget( mpEditTitle,     1, 1, 1, 5 );
   netLayout->addWidget( mpEditAlbum,     2, 1, 1, 5 );
   netLayout->addWidget( mpEditTrackNr,   3, 1 );
   netLayout->addWidget( mpEditYear,      3, 3 );
   netLayout->addWidget( mpEditGenre,     3, 5 );

   netLayout->setColumnStretch( 0,  1 );
   netLayout->setColumnStretch( 1,  2 );
   netLayout->setColumnStretch( 2,  1 );
   netLayout->setColumnStretch( 3, 14 );
   netLayout->setColumnStretch( 4,  1 );
   netLayout->setColumnStretch( 5, 81 );

#if QT_VERSION < 0x040300
   netLayout->setMargin( 2 );
#else
   netLayout->setContentsMargins( 2, 2, 2, 2 );
#endif
   mpTagGroupBox->setLayout( netLayout );
   
   mpDatabaseGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *databaseLayout = new QGridLayout;
   databaseLayout->addWidget( mpSelectFlavor, 0, 0 );
   
#if QT_VERSION < 0x040300
   databaseLayout->setMargin( 2 );
#else
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
            
   connect( mpSelectFlavor, SIGNAL(currentIndexChanged(int)),
            this, SLOT(handleFlavor(int)) );
}


void InfoEdit::recurse( const QDir &dir, bool isBase )
{
   QFileInfoList files(dir.entryInfoList());
   int i;
   
   if( isBase )
   {
      if( (mRecurseMode == MODE_SETTAGS) && mRecurseArtist.isEmpty() &&
          mRecurseTitle.isEmpty() && mRecurseAlbum.isEmpty() && 
          mRecurseYear.isEmpty() && mRecurseGenre.isEmpty() && 
          (mRecurseFlavor == 3) )
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
               if( mRecurseFlavor < 3 )
               {
                  mpSelectFlavor->setCurrentIndex( mRecurseFlavor );
               }
               QCoreApplication::processEvents();
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
   
   if( fileInfo.isFile() )
   {
      mIsValid = true;
      mIsFile  = true;
      
      mpDatabase->getTrackInfoByFileName( &mTrackInfo, fullpath );
      addNoFlavorChange( false );
      
      int flavor = 0;
      if( mTrackInfo.isFlagged( TrackInfo::Favorite ) )
      {
         flavor = 1;
      }
      if( mTrackInfo.isFlagged( TrackInfo::Unwanted ) )
      {
         flavor = 2;
      }
      mpSelectFlavor->setCurrentIndex( flavor );
      
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
         addNoFlavorChange( true );
      }
      else
      {
         mpShowPathName->clear();
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
         mRecurseArtist = mpEditArtist->text();
         mRecurseTitle  = mpEditTitle->text();
         mRecurseAlbum  = mpEditAlbum->text();
         mRecurseYear   = mpEditYear->text();
         mRecurseGenre  = mpEditGenre->text();
         mRecurseFlavor = mpSelectFlavor->currentIndex();
         
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




void InfoEdit::handleFlavor( int index )
{
   switch( index )
   {
      case 0:
         mTrackInfo.setFlag( TrackInfo::Unwanted, false );
         break;
      case 1:
         mTrackInfo.setFlag( TrackInfo::Favorite, true );
         break;
      case 2:
         mTrackInfo.setFlag( TrackInfo::Unwanted, true );
         break;
      default:
         break;
   }
   
   handleChange();
}


void InfoEdit::addNoFlavorChange( bool add )
{
   int count = mpSelectFlavor->count();
   if( add )
   {
      if( count == 3 )
      {
         mpSelectFlavor->addItem( QString( tr("Don't Change") ) );
         mpSelectFlavor->setCurrentIndex( 3 );
      }
   }
   else
   {
      if( count == 4 )
      {
         mpSelectFlavor->removeItem( 3 );
      }
   }
}
