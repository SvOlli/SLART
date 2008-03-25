/**
 * CDEdit.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CDEDIT_HPP
#define CDEDIT_HPP CDEDIT_HPP

#include <QWidget>

class QScrollArea;
class QGridLayout;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class CDToc;
class CDDB;

class CDEdit : public QWidget
{
Q_OBJECT

public:
   CDEdit( CDToc *toc, CDDB *cddb, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   void trackInfo( int tracknr, bool *dorip, bool *doenqueue, QString *artist, QString *title,
                   QString *albumartist, QString *albumtitle, QString *genre, int *year );

   void updateCDText( int track, const QString &artist, const QString &title );
   
   void clear();

public slots:
   void handleTrackNr();
   void handleEnqueueTrack();
   void handleTrackArtist();
   void handleNormalizeTitle();
   void handleTrackYear();
   void setTrackHidden( int track, bool hide );
   void setTrackDisabled( int track, bool disabled );
   void update();
   void updateCDDB();
   void splitTitles();
   void ensureVisible( int tracknr );

private:
   CDEdit( const CDEdit &other );
   CDEdit &operator=( const CDEdit &other );

   CDToc        *mpToc;
   CDDB         *mpCDDB;
   QScrollArea  *mpScrollArea;
   QWidget      *mpScrollWidget;
   QGridLayout  *mpMainLayout;
   QLabel       *mpLabelDiscArtist;
   QLabel       *mpLabelDiscTitle;
   QLabel       *mpLabelGenre;
   QLineEdit    *mpDiscArtist;
   QLineEdit    *mpDiscTitle;
   QLineEdit    *mpGenre;
   QLabel       *mpLabelDiscID;
   QLabel       *mpDiscID;
   QLabel       *mpLabelTrackNr;
   QLabel       *mpLabelEnqueueTrack;
   QLabel       *mpLabelTrackArtist;
   QLabel       *mpLabelTrackTitle;
   QLabel       *mpLabelTrackYear;
   QLabel       *mpLabelTrackPlaytime;
   QCheckBox    **mpTrackNr;
   QCheckBox    **mpEnqueueTrack;
   QLineEdit    **mpTrackArtist;
   QLineEdit    **mpTrackTitle;
   QLineEdit    **mpTrackYear;
   QLabel       **mpTrackPlaytime;
   QComboBox    *mpSplitMode;
   QPushButton  *mpSplitButton;
   QPushButton  *mpToggleRipButton;
   QPushButton  *mpToggleEnqueueButton;
   QPushButton  *mpCopyArtistButton;
   QPushButton  *mpNormalizeTitleButton;
   QPushButton  *mpCopyYearButton;
};

#endif
