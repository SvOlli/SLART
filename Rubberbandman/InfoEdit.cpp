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
#include "Trace.hpp"

#define MODE_NOTHING     0
#define MODE_SETTAGS     1
#define MODE_NORM_ARTIST 2
#define MODE_NORM_TITLE  3


InfoEdit::InfoEdit( QWidget *parent )
: QWidget( parent )
, mpButtonSet        ( new QPushButton )
, mpButtonNormArtist ( new QPushButton( tr("Norm. Artist") ) )
, mpButtonNormTitle  ( new QPushButton( tr("Norm. Title") ) )
, mpGridGroupBox     ( new QGroupBox( tr("OggVorbis Information") ) )
, mpLabelPathName    ( new QLabel( tr("Path:") ) )
, mpLabelFileName    ( new QLabel( tr("File:") ) )
, mpLabelArtist      ( new QLabel( tr("Artist:") ) )
, mpLabelTitle       ( new QLabel( tr("Title:") ) )
, mpLabelAlbum       ( new QLabel( tr("Album:") ) )
, mpLabelTrackNr     ( new QLabel( tr("TrackNr:") ) )
, mpLabelYear        ( new QLabel( tr("Year:") ) )
, mpLabelGenre       ( new QLabel( tr("Genre:") ) )
, mpShowPathName     ( new QLineEdit )
, mpShowFileName     ( new QLineEdit )
, mpEditArtist       ( new QLineEdit )
, mpEditTitle        ( new QLineEdit )
, mpEditAlbum        ( new QLineEdit )
, mpEditTrackNr      ( new QLineEdit )
, mpEditYear         ( new QLineEdit )
, mpEditGenre        ( new QLineEdit )
, mRecurseMode       ( 0 )
, mIsValid           ( false )
, mIsFile            ( false )
, mTagList           ( )
, mFileName          ( )
, mRecurseArtist     ( )
, mRecurseTitle      ( )
, mRecurseAlbum      ( )
, mRecurseYear       ( )
, mRecurseGenre      ( )
{
   
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

   mpGridGroupBox->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
   QGridLayout *layout = new QGridLayout;
   
   layout->addWidget( mpLabelPathName, 0, 0 );
   layout->addWidget( mpLabelFileName, 1, 0 );
   layout->addWidget( mpLabelArtist,   2, 0 );
   layout->addWidget( mpLabelTitle,    3, 0 );
   layout->addWidget( mpLabelAlbum,    4, 0 );
   layout->addWidget( mpLabelTrackNr,  5, 0 );
   layout->addWidget( mpLabelYear,     5, 2 );
   layout->addWidget( mpLabelGenre,    5, 4 );
   
   layout->addWidget( mpShowPathName,  0, 1, 1, 5 );
   layout->addWidget( mpShowFileName,  1, 1, 1, 5 );
   layout->addWidget( mpEditArtist,    2, 1, 1, 5 );
   layout->addWidget( mpEditTitle,     3, 1, 1, 5 );
   layout->addWidget( mpEditAlbum,     4, 1, 1, 5 );
   layout->addWidget( mpEditTrackNr,   5, 1 );
   layout->addWidget( mpEditYear,      5, 3 );
   layout->addWidget( mpEditGenre,     5, 5 );

   layout->setColumnStretch( 0,  1 );
   layout->setColumnStretch( 1,  2 );
   layout->setColumnStretch( 2,  1 );
   layout->setColumnStretch( 3, 14 );
   layout->setColumnStretch( 4,  1 );
   layout->setColumnStretch( 5, 81 );

   layout->setMargin( 2 );
   mpGridGroupBox->setLayout(layout);
   
   QHBoxLayout *hlayout = new QHBoxLayout;
   hlayout->addWidget( mpButtonSet );
   hlayout->addWidget( mpButtonNormArtist );
   hlayout->addWidget( mpButtonNormTitle );
   
   QVBoxLayout *vlayout = new QVBoxLayout;
   vlayout->addLayout( hlayout );
   vlayout->addWidget( mpGridGroupBox );
   vlayout->setMargin( 2 );
   setLayout( vlayout );
   
   setMaximumHeight( vlayout->minimumSize().height() );
   load(QString());
   
   connect( mpButtonSet, SIGNAL(clicked()),
            this, SLOT(setSave()) );
   connect( mpButtonNormArtist, SIGNAL(clicked()),
            this, SLOT(normalizeArtist()) );
   connect( mpButtonNormTitle,  SIGNAL(clicked()),
            this, SLOT(normalizeTitle()) );
}


void InfoEdit::recurse( const QDir &dir, bool isBase )
{
   QFileInfoList files(dir.entryInfoList());
   int i;
   
   if( isBase )
   {
      if( (mRecurseMode == MODE_SETTAGS) && mRecurseArtist.isEmpty() &&
          mRecurseTitle.isEmpty() && mRecurseAlbum.isEmpty() && 
          mRecurseYear.isEmpty() && mRecurseGenre.isEmpty() )
      {
         return;
      }
      
      mpButtonSet->setDisabled( true );
      mpButtonNormArtist->setDisabled( true );
      mpButtonNormTitle->setDisabled( true );
   }
   
   for( i = 0; i < files.size(); i++ )
   {
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
   
   if( isBase )
   {
      mpButtonSet->setDisabled( false );
      mpButtonNormArtist->setDisabled( false );
      mpButtonNormTitle->setDisabled( false );
         
      mpShowFileName->clear();
      mpEditArtist->clear();
      mpEditTitle->clear();
      mpEditAlbum->clear();
      mpEditTrackNr->clear();
      mpEditYear->clear();
      mpEditGenre->clear();
   }
}


void InfoEdit::normalizeArtist()
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


void InfoEdit::normalizeTitle()
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
      }
   }
   
   if( mIsValid )
   {
      mpButtonSet->setEnabled( mIsValid );
   }
   
   if( mIsFile )
   {
      mpButtonSet->setText( tr("Save Tags") );
      mpEditTrackNr->setDisabled( false );
      mpShowFileName->setDisabled( false );
   }
   else
   {
      mpButtonSet->setText( tr("Set Recursive") );
      mpEditTrackNr->setDisabled( true );
      mpShowFileName->setDisabled( true );
   }
#if 0
TRACEMSG << "mIsValid" << mIsValid << "mIsFile" << mIsFile;
#endif
}

void InfoEdit::setSave()
{
#if 0
TRACESTART(InfoEdit::setSave)
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
         
         mRecurseMode = MODE_SETTAGS;
         recurse( mFileName );
         mRecurseMode = MODE_NOTHING;
      }
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
      newname = mTagList.fileName( "|$ARTIST| - |$TITLE|" );
   }
   else
   {
      newname = mTagList.fileName( "(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" );
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
}


QString InfoEdit::tagsFileName( const QString &pattern, bool filterPath )
{
   return mTagList.fileName( pattern, filterPath );
}


QString InfoEdit::fileName()
{
   return mFileName;
}
