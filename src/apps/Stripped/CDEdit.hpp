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
class CDInfo;
class CDDBClient;

/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief widget to edit cd description

*/
class CDEdit : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param info pointer to data container
    \param cddbClient pointer to CDDBClient
    \param parent parent widget
   */
   CDEdit( CDInfo *info, CDDBClient *cddbClient, QWidget *parent = 0 );
   /*!
    \brief destructor
    */
   virtual ~CDEdit();

   /*!
    \brief get the track info by tracknr

    \param tracknr track number to get data for
    \param dorip indicator if track should be ripped
    \param doenqueue indicator if track should be enqueued after ripping
    \param artist name of artist
    \param title name of track title
    \param albumartist name of album artist
    \param albumtitle name of album title
    \param genre genre description
    \param year year of creation
   */
   void trackInfo( int tracknr, bool *dorip, bool *doenqueue, QString *artist, QString *title,
                   QString *albumartist, QString *albumtitle, QString *genre, int *year );

   /*!
    \brief update function for reading of cdtext

    \param track track number
    \param artist name of artist
    \param title name of track title
   */
   void updateCDText( int track, const QString &artist, const QString &title );

   /*!
    \brief clear the sheet

   */
   void clear();

   /*!
    \brief check if the sheet is empty

   */
   bool isEmpty();

   /*!
    \brief move the focus up or down by one line

    Callback for CDEditCheckBox and CDEditLineEdit.

    \param widget widget invoking callback
    \param isUp indicator if key was up (true) of down (false)
   */
   void keyUpDown( QWidget *widget, bool isUp );

signals:

   /*!
    \brief signalize if the TOC contains data

    \param flag data available
   */
   void containsData( bool flag );

public slots:

   /*!
    \brief handle toggle all tracks button

   */
   void handleTrackNr();

   /*!
    \brief handle toggle enqueue button

   */
   void handleEnqueueTrack();

   /*!
    \brief handle copy track artist button

   */
   void handleTrackArtist();

   /*!
    \brief handle normalize title button

   */
   void handleNormalizeTitle();

   /*!
    \brief handle copy track year button

   */
   void handleTrackYear();

   /*!
    \brief hide a track from sheet

    \param track track number
    \param hide indicator if track should be hidden or not
   */
   void setTrackHidden( int track, bool hide );

   /*!
    \brief disable a track on sheet (being read)

    \param track track number
    \param disabled indicator if track should be disabled or not
   */
   void setTrackDisabled( int track, bool disabled );

   /*!
    \brief update sheet

   */
   void update();

   /*!
    \brief ensure that a certain track is visible on sheet

    \param tracknr track number
   */
   void ensureVisible( int track );

private:
   Q_DISABLE_COPY( CDEdit )

   /*!
    \brief make sure that a widget is in visible area

    \param widget widget to show
   */
   void ensureVisibleFocus( QWidget *widget );

   CDInfo       *mpCDInfo; /*!< TODO */
   CDDBClient   *mpCDDBClient; /*!< TODO */
   QScrollArea  *mpScrollArea; /*!< TODO */
   QWidget      *mpScrollWidget; /*!< TODO */
   QGridLayout  *mpMainLayout; /*!< TODO */
   QLabel       *mpLabelDiscArtist; /*!< TODO */
   QLabel       *mpLabelDiscTitle; /*!< TODO */
   QLabel       *mpLabelDiscGenre; /*!< TODO */
   QLineEdit    *mpDiscArtist; /*!< TODO */
   QLineEdit    *mpDiscTitle; /*!< TODO */
   QLineEdit    *mpDiscGenre; /*!< TODO */
   QLabel       *mpLabelDiscID; /*!< TODO */
   QLabel       *mpDiscID; /*!< TODO */
   QLabel       *mpDiscPlaytime; /*!< TODO */
   QLabel       *mpLabelTrackNr; /*!< TODO */
   QLabel       *mpLabelEnqueueTrack; /*!< TODO */
   QLabel       *mpLabelTrackArtist; /*!< TODO */
   QLabel       *mpLabelTrackTitle; /*!< TODO */
   QLabel       *mpLabelTrackYear; /*!< TODO */
   QLabel       *mpLabelTrackPlaytime; /*!< TODO */
   QCheckBox    **mpTrackNr; /*!< TODO */
   QCheckBox    **mpEnqueueTrack; /*!< TODO */
   QLineEdit    **mpTrackArtist; /*!< TODO */
   QLineEdit    **mpTrackTitle; /*!< TODO */
   QLineEdit    **mpTrackYear; /*!< TODO */
   QLabel       **mpTrackPlaytime; /*!< TODO */
   QPushButton  *mpToggleRipButton; /*!< TODO */
   QPushButton  *mpToggleEnqueueButton; /*!< TODO */
   QPushButton  *mpCopyArtistButton; /*!< TODO */
   QPushButton  *mpNormalizeTitleButton; /*!< TODO */
   QPushButton  *mpCopyYearButton; /*!< TODO */
   int          mLastColumn; /*!< TODO */
};

/*! @} */

#endif
