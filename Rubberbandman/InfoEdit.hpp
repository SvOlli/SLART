/**
 * InfoEdit.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef INFOEDIT_HPP
#define INFOEDIT_HPP INFOEDIT_HPP

#include <QWidget>
#include <QString>
#include "TagList.hpp"
#include "TrackInfo.hpp"
   
class QGroupBox;
class QLabel;
class QLineEdit;
class QIntValidator;
class QPushButton;
class QDir;
class Database;

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

signals:
   void fileStats( bool isValid, bool isFile );
   
private:
   InfoEdit( const InfoEdit &other );
   InfoEdit &operator=( const InfoEdit &other );

   /* normalize a line edit field */
   void normalize( QLineEdit *lineEdit );
   /* save the currently edited tags */
   void saveFile();
   /* walk through the directory tree */
   void recurse( const QDir &dir, bool isBase = true );

   Database    *mpDatabase;
   TrackInfo   mTrackInfo;

   QPushButton *mpButtonSet;
   QPushButton *mpButtonNormArtist;
   QPushButton *mpButtonNormTitle;
   QPushButton *mpButtonCancel;

   QGroupBox *mpGridGroupBox;

   QLabel *mpLabelPathName;
   QLabel *mpLabelFileName;
   QLabel *mpLabelArtist;
   QLabel *mpLabelTitle;
   QLabel *mpLabelAlbum;
   QLabel *mpLabelTrackNr;
   QLabel *mpLabelYear;
   QLabel *mpLabelGenre;
   
   QLineEdit *mpShowPathName;
   QLineEdit *mpShowFileName;
   QLineEdit *mpEditArtist;
   QLineEdit *mpEditTitle;
   QLineEdit *mpEditAlbum;
   QLineEdit *mpEditTrackNr;
   QLineEdit *mpEditYear;
   QLineEdit *mpEditGenre;
   QIntValidator *mpValidateTrackNr;
   QIntValidator *mpValidateYear;

   int     mRecurseMode;
   bool    mIsValid;
   bool    mIsFile;
   bool    mCancel;
   TagList mTagList;
   QString mFileName;
   QString mRecurseArtist;
   QString mRecurseTitle;
   QString mRecurseAlbum;
   QString mRecurseYear;
   QString mRecurseGenre;
};

#endif
