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
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCompleter;
class QWidget;

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
   enum Flag {
      ScannedWithPeak  = 0x00000001,
      ScannedWithPower = 0x00000002,
      Unwanted         = 0x00000004,
      Favorite         = 0x00000008,
      None             = 0x00000000
   };

   /*!
    \brief \todo complete documentation

   */
   TrackInfo();
   /*!
    \brief

    \param that
   */
   TrackInfo( const TrackInfo &that );
   /*!
    \brief

    \param that
   */
   TrackInfo &operator=( const TrackInfo &that );
   /*!
    \brief

    \param that
   */
   bool operator==( const TrackInfo &that ) const;
   /*!
    \brief

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
    \param folders
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
              const QString &folders = QString(),
              unsigned int   flags = 0,
              unsigned int   id = 0 );

   unsigned int mID;
   QString      mDirectory;
   QString      mFileName;
   QString      mArtist;
   QString      mTitle;
   QString      mAlbum;
   int          mTrackNr;
   int          mYear;
   QString      mGenre;
   unsigned int mPlayTime;
   unsigned int mLastScanned;
   unsigned int mLastTagsRead;
   unsigned int mTimesPlayed;
   double       mVolume;
   QString      mFolders;
   unsigned int mFlags;

   /*!
    \brief clear all data

   */
   void clear();

   /*!
    \brief mID > 0 when track is in database

   */
   bool isInDatabase();

   /*!
    \brief set or unset a flag

   */
   void setFlag( Flag flag, bool set );
   /*!
    \brief check if a flag is set

   */
   bool isFlagged( Flag flag );
   /*!
    \brief for combined flags get the mask

   */
   unsigned int getFlagMask( Flag flag );

   /*!
    \brief add or remove the track to a folder

   */
   void setFolder( const QString &folder, bool add );
   /*!
    \brief check if the track is in a folder

   */
   bool isInFolder( const QString &folder );
   /*!
    \brief get a list of all folders the track is in

   */
   QStringList getFolders();

   /*!
    \brief return full path of file

   */
   QString filePath() const;
   /*!
    \brief put it all together in a string for debugging purposes

   */
   QString toString() const;
   /*!
    \brief format to a string by patterns

   */
   QString displayString( const QString &pattern ) const;

   /*!
    \brief convert seconds to a 0:00 timecode

   */
   static QString sec2minsec( int seconds );

private:
   /*!
    \brief subroutine for displayString

   */
   QString valueByKey( const QString &key ) const;
};

/*! @} */

typedef QList<TrackInfo> TrackInfoList;


#endif
