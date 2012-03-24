/*
 * src/apps/Stripped/CDInfo.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDINFO_HPP
#define CDINFO_HPP CDINFO_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief data storage class for cd and track information

*/
class CDInfo
{
public:
   /*!
    \brief constructor

   */
   CDInfo();

   /*!
    \brief destructor

   */
   virtual ~CDInfo();

   /*!
    \brief reset data

   */
   void clear();

   /*!
    \brief set the global disc data (also resets all other data)

    \param tracks
    \param startSector
    \param endSector
   */
   void setDisc( int tracks, int startSector, int endSector );

   /*!
    \brief set the data for a track

    \param track
    \param startSector
    \param endSector
    \param isAudio
    \param copyProhib
    \param preEmp
    \param twoChannels
   */
   void setTrack( int track, int startSector, int endSector,
                  bool isAudio, bool copyProhib, bool preEmp, bool twoChannels );

   /*!
    \brief set the artist for a track

    \param track track number to set artist (-1: disc)
    \param artist artist name to set / append
    \param append data should be appended instead of replaced
   */
   void setArtist( int track = -1, const QString &artist = QString(), bool append = false );

   /*!
    \brief set the title for a track

    \param track track number to set title (-1: disc)
    \param title track title to set / append
    \param append data should be appended instead of replaced
   */
   void setTitle( int track = -1, const QString &title = QString(), bool append = false );

   /*!
    \brief set the genre of the disc

    \param genre to set / append
    \param append data should be appended instead of replaced
   */
   void setGenre( const QString &genre = QString(), bool append = false );

   /*!
    \brief set the year of the disc

    \param year year of the disc
   */
   void setYear( int year );

   /*!
    \brief split the title to artist and title according to separator

    \param separator the separator to set, e.g. " / "
   */
   void splitTitle( const QString &separator );

   /*!
    \brief calculate the cddb discid

   */
   void calcCddbDiscID();

   /*!
    \brief return the cddb discid

   */
   QString cddbDiscID();

   /*!
    \brief return the number of tracks

   */
   int tracks();

   /*!
    \brief return the first sector of a track or disc if no track is given

    \param track track number to get sector for (-1: disc)
   */
   int firstSector( int track = -1 );

   /*!
    \brief return the last sector of a track or disc if no track is given

    \param track track number to get sector for (-1: disc)
   */
   int lastSector( int track = -1 );

   /*!
    \brief return the length of a track or disc if no track is given if mm:ss format

    \param track track number to get length for (-1: disc)
   */
   QString length( int track = -1 );

   /*!
    \brief return if track is audio

    \param track track number
   */
   bool isAudio( int track );

   /*!
    \brief get the title of a track or disc, if no parameter

    \param track track number to get title for (-1: disc)
   */
   QString title( int track = -1 );

   /*!
    \brief get the artist of a track or disc, if no parameter

    \param track track number to get artist name for (-1: disc)
   */
   QString artist( int track = -1 );

   /*!
    \brief get the genre of the disc

   */
   QString genre();

   /*!
    \brief get the year of the disc

   */
   int year();

private:
   Q_DISABLE_COPY( CDInfo )

   int            mNumTracks; /*!< \brief disc info: number of tracks */
   int            mDiscStartSector; /*!< \brief disc info: disc start sector */
   int            mDiscEndSector; /*!< \brief disc info: disc end sector */
   unsigned long  mDiscID; /*!< \brief disc info: cddb disc id */
   int            mDiscYear; /*!< \brief disc info: disc year */
   QString        *mpDiscGenre; /*!< \brief disc info: genre */
   QString        *mpDiscArtist; /*!< \brief disc info: album artist name */
   QString        *mpDiscTitle; /*!< \brief disc info: album title */

   int            *mpTrackStartSector; /*!< \brief tracks info: track start sector */
   int            *mpTrackEndSector; /*!< \brief tracks info: track end sector */
   bool           *mpTrackIsAudio; /*!< \brief tracks info: track is audio */
   bool           *mpTrackCopyProhib; /*!< \brief tracks info: copying of track is prohibited */
   bool           *mpTrackPreEmp; /*!< \brief tracks info: track pre emphasis */
   bool           *mpTrack2Channels; /*!< \brief tracks info: track is two channels */
   QString        **mpTrackArtist; /*!< \brief tracks info: track artist name */
   QString        **mpTrackTitle; /*!< \brief tracks info: track title */
};

/*! @} */

#endif
