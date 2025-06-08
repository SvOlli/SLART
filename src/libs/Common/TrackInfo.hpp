/*
 * src/libs/Common/TrackInfo.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACKINFO_HPP
#define TRACKINFO_HPP TRACKINFO_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QMetaType>
#include <QProcessEnvironment>
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class TrackInfo
{
public:
   /*!
    \brief flags for special states of entry

   */
   enum Flag {
      ScannedWithPeak  = 0x00000001,
      ScannedWithPower = 0x00000002,
      Unwanted         = 0x00000004,
      Favorite         = 0x00000008,
      None             = 0x00000000
   };

   /*!
    \brief constructor

   */
   TrackInfo();

   /*!
    \brief copy constructor

    \param that
   */
   TrackInfo( const TrackInfo &that );

   /*!
    \brief assignment operator

    \param that
   */
   TrackInfo &operator=( const TrackInfo &that );

   /*!
    \brief is equal comparision operator

    \param that
   */
   bool operator==( const TrackInfo &that ) const;

   /*!
    \brief is not equal comparision operator

    \param that
   */
   bool operator!=( const TrackInfo &that ) const;

   /*!
    \brief constructor

    \param directory
    \param filename
    \param artist
    \param title
    \param album
    \param tracknr
    \param year
    \param genre
    \param playtime
    \param lastscanned
    \param lasttagsread
    \param timesplayed
    \param volume
    \param groups
    \param flags
    \param id
   */
   TrackInfo( const QString &directory,
              const QString &filename,
              const QString &artist,
              const QString &title,
              const QString &album,
              int            tracknr,
              int            year,
              const QString &genre,
              unsigned int   playtime,
              unsigned int   lastscanned,
              unsigned int   lasttagsread,
              unsigned int   timesplayed = 0,
              double         volume = 0.0,
              const QString &groups = QString(),
              unsigned int   flags = 0,
              unsigned int   id = 0 );

   unsigned int mID; /*!< \brief database id */
   QString      mDirectory; /*!< \brief directory of file */
   QString      mFileName; /*!< \brief name of file TODO */
   QString      mArtist; /*!< \brief artist name */
   QString      mTitle; /*!< \brief track title name */
   QString      mAlbum; /*!< \brief album name */
   int          mTrackNr; /*!< \brief track number (-1 = not set) */
   int          mYear; /*!< \brief year (-1 = not set) */
   QString      mGenre; /*!< \brief genre of track */
   unsigned int mPlayTime; /*!< \brief play time of track in seconds */
   unsigned int mLastScanned; /*!< \brief timestamp of last scanning of track */
   unsigned int mLastTagsRead; /*!< \brief timestamp of the last time the tags were read */
   unsigned int mTimesPlayed; /*!< \brief number of times played */
   double       mVolume; /*!< \brief volume adjustment factor */
   QString      mGroups; /*!< \brief groups associated to track */
   unsigned int mFlags; /*!< \brief flags of track */

   /*!
    \brief clear all data

   */
   void clear();

   /*!
    \brief mID > 0 when track is in database

    \return bool
   */
   bool isInDatabase() const;

   /*!
    \brief set or unset a flag

    \param flag
    \param set
   */
   void setFlag( Flag flag, bool set );

   /*!
    \brief check if a flag is set

    \param flag
    \return bool
   */
   bool isFlagged( Flag flag ) const;

   /*!
    \brief for combined flags get the mask

    \param flag
    \return unsigned int
   */
   unsigned int getFlagMask( Flag flag ) const;

   /*!
    \brief add or remove the track to a group

    \param group
    \param add
   */
   void setGroup( const QString &group, bool add );

   /*!
    \brief check if the track is in a group

    \param group
    \return bool
   */
   bool isInGroup( const QString &group ) const;

   /*!
    \brief get a list of all groups the track is in

    \return QStringList
   */
   QStringList getGroups() const;

   /*!
    \brief return full path of file

    \return QString
   */
   QString filePath() const;

   /*!
    \brief put it all together in a string for debugging purposes

    \return QString
   */
   QString toString() const;

   /*!
    \brief format to a string by patterns

    \param pattern
    \return QString
   */
   QString displayString( const QString &pattern ) const;

   /*!
    \brief convert to environment variables

    \return QProcessEnvironment
   */
   QProcessEnvironment toProcessEnvironment() const;

   /*!
    \brief convert seconds to a 0:00 timecode

    \param seconds
    \return QString
   */
   static QString sec2minsec( int seconds );

private:
   /*!
    \brief subroutine for displayString

    \param key
    \return QString
   */
   QString valueByKey( const QString &key ) const;
};

/*! @} */

typedef QList<TrackInfo> TrackInfoList;

Q_DECLARE_METATYPE(TrackInfo)
Q_DECLARE_METATYPE(TrackInfoList)

#endif
