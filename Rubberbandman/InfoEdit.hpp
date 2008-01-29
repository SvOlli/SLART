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
   
class QGroupBox;
class QLabel;
class QLineEdit;
class QIntValidator;
class QPushButton;
class QDir;

class InfoEdit : public QWidget
{
Q_OBJECT

public:
   InfoEdit( QWidget *parent = 0 );

public slots:
   void load( const QString &fullpath = QString() );
   void setSave();
   void normalizeArtist();
   void normalizeTitle();

signals:
   void fileStats( bool isValid, bool isFile );
   
private:
   InfoEdit( const InfoEdit &other );
   InfoEdit &operator=( const InfoEdit &other );
   void normalize( QLineEdit *lineEdit );
   void saveFile();
   void recurse( const QDir &dir );

   QPushButton *mpButtonSet;
   QPushButton *mpButtonNormArtist;
   QPushButton *mpButtonNormTitle;

   QGroupBox *mpGridGroupBox;

   QLabel *mpLabelPathName;
   QLabel *mpLabelFileName;
   QLabel *mpLabelArtist;
   QLabel *mpLabelTitle;
   QLabel *mpLabelAlbum;
   QLabel *mpLabelTrackNr;
   QLabel *mpLabelYear;
   QLabel *mpLabelGenre;
   
   QLabel *mpShowPathName;
   QLabel *mpShowFileName;
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
   QString mFileName;
};

#endif
