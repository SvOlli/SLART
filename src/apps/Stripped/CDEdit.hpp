/**
 * src/apps/Stripped/CDEdit.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDEDIT_HPP
#define CDEDIT_HPP CDEDIT_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QScrollArea;

/* forward declaration of local classes */
class CDToc;
class CDDB;


class CDEdit : public QWidget
{
Q_OBJECT
   
public:
   CDEdit( CDToc *toc, CDDB *cddb, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
   /* get the track info by tracknr */
   void trackInfo( int tracknr, bool *dorip, bool *doenqueue, QString *artist, QString *title,
                   QString *albumartist, QString *albumtitle, QString *genre, int *year );
   /* update function for reading of cdtext */
   void updateCDText( int track, const QString &artist, const QString &title );
   /* clear the sheet */
   void clear();
   /* check if the sheet is empty */
   bool isEmpty();
   
signals:
   /* signalize if the TOC contains data */
   void containsData( bool flag );
   
public slots:
   /* handle toggle all tracks button */
   void handleTrackNr();
   /* handle toggle enqueue button */
   void handleEnqueueTrack();
   /* handle copy track artist button */
   void handleTrackArtist();
   /* handle normalize title button */
   void handleNormalizeTitle();
   /* handle copy track year button */
   void handleTrackYear();
   /* hide a track from sheet */
   void setTrackHidden( int track, bool hide );
   /* disable a track on sheet (being read) */
   void setTrackDisabled( int track, bool disabled );
   /* update sheet */
   void update( bool useCDDB );
   /* copy info read from cddb from toc to sheet */
   void updateCDDB();
   /* handle split titles button */
   void splitTitles();
   /* ensure that a certain track is visible on sheet */
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
   QLabel       *mpDiscPlaytime;
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
