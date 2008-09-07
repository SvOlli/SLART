/**
 * InfoEdit.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef INFOEDIT_HPP
#define INFOEDIT_HPP INFOEDIT_HPP

#include <QWidget>

#include <QStringList>

#include "TagList.hpp"
#include "TrackInfo.hpp"
   
class QAction;
class QCheckBox;
class QComboBox;
class QDir;
class QGroupBox;
class QIntValidator;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;

class Database;
class ScrollLine;


class InfoEdit : public QWidget
{
Q_OBJECT
   
public:
   InfoEdit( Database *database, QWidget *parent = 0 );
   QString tagsFileName( const QString &pattern, bool filterPath = true );
   QString fileName();
   
public slots:
   /* load tags from a file */
   void load( const QString &fullpath = QString() );
   /* handle save button */
   void handleSetSave();
   /* handle norm. artist button */
   void handleNormalizeArtist();
   /* handle norm. title button */
   void handleNormalizeTitle();
   /* handle cancel button */
   void handleCancel();
   /* enable save button */
   void handleChange();
   /* handle the flags menu */
   void handleFlagsMenu( QAction *action );
   /* handle the folders menu */
   void handleFoldersMenu( QAction *action );
   
signals:
   void fileStats( bool isValid, bool isFile );
   void updated();
   
private:
   InfoEdit( const InfoEdit &other );
   InfoEdit &operator=( const InfoEdit &other );
   
   /* normalize a line edit field */
   void normalize( QLineEdit *lineEdit );
   /* load tags from a file */
   void loadFile( const QString &fullpath );
   /* save the currently edited tags */
   void saveFile();
   /* walk through the directory tree */
   void recurse( const QDir &dir, bool isBase = true );
   /* update the info of the track concerning flags and folders */
   void updateDatabaseInfo( bool withRecurse );
   
   Database    *mpDatabase;
   TrackInfo   mTrackInfo;
   
   QPushButton *mpButtonSet;
   QPushButton *mpButtonNormArtist;
   QPushButton *mpButtonNormTitle;
   QPushButton *mpButtonCancel;
   
   QGroupBox *mpFileGroupBox;
   QGroupBox *mpTagGroupBox;
   QGroupBox *mpDatabaseGroupBox;
   
   ScrollLine *mpShowPathName;
   ScrollLine *mpShowFileName;
   ScrollLine *mpShowSize;
   ScrollLine *mpShowPlayTime;
   
   QLineEdit *mpEditArtist;
   QLineEdit *mpEditTitle;
   QLineEdit *mpEditAlbum;
   QLineEdit *mpEditTrackNr;
   QLineEdit *mpEditYear;
   QLineEdit *mpEditGenre;
   QIntValidator *mpValidateTrackNr;
   QIntValidator *mpValidateYear;
   
   QPushButton  *mpButtonFlags;
   QMenu        *mpMenuFlags;
   QLabel       *mpShowTimesPlayed;
   QPushButton  *mpButtonFolders;
   QMenu        *mpMenuFolders;
   QAction      *mpRecurseSetFlags;
   QAction      *mpRecurseUnsetFlags;
   QAction      *mpFavoriteTrackFlag;
   QAction      *mpUnwantedTrackFlag;
   QAction      *mpTrackScannedFlag;
   QAction      *mpRecurseSetFolders;
   QAction      *mpRecurseUnsetFolders;
   
   int          mRecurseMode;
   bool         mIsValid;
   bool         mIsFile;
   bool         mCancel;
   TagList      mTagList;
   QString      mFileName;
   QString      mRecurseArtist;
   QString      mRecurseTitle;
   QString      mRecurseAlbum;
   QString      mRecurseYear;
   QString      mRecurseGenre;
   bool         mRecurseSetFlags;
   bool         mRecurseUnsetFlags;
   bool         mRecurseFavoriteTrackFlag;
   bool         mRecurseUnwantedTrackFlag;
   bool         mRecurseTrackScannedFlag;
   bool         mRecurseSetFolders;
   bool         mRecurseUnsetFolders;
   QStringList  mRecurseFolders;
};

#endif
