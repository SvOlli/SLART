/*
 * src/apps/Funkytown/TheMagic.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef THEMAGIC_HPP
#define THEMAGIC_HPP THEMAGIC_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QBuffer;
class QFile;
class QListWidget;
class QListWidgetItem;

/* forward declaration of local classes */
class MagicQueue;
class Satellite;


/*!
  \addtogroup Funkytown

  @{
*/

/*!
 \brief \todo complete documentation

*/
class TheMagic : public QObject
{
   Q_OBJECT

   friend class MagicQueue;

public:
   TheMagic( MagicQueue *magicQueue = 0 );
   virtual ~TheMagic();

   TheMagic( const TheMagic &that );

   /*!
    \brief the magic that gonna happen before the download

   */
   void preDownload();
   /*!
    \brief the magic that gonna happen after the download

   */
   void postDownload( bool success );
   /*!
    \brief return filename for the file that being downloaded, if any

   */
   QString fileName();
   /*!
    \brief return referer, if any

   */
   QString referer();
   /*!
    \brief note that the download has failed

   */
   void fail();
   /*!
    \brief sset the content (mime-)type, used for 8bit conversion

   */
   void setContentType( const QString &contentType );
   /*!
    \brief the device handler to write the data to

   */
   QIODevice *ioDevice();

   /*!
    \brief the url to download

   */
   QString          mURL;
   /*!
    \brief marker if the list entry has been selected (for sending to Partyman)

   */
   bool             mSelected;


private:
   TheMagic &operator=( const TheMagic &that );

   /*!
    \brief the different states of downloading and analysing

   */
   enum { stageFresh, stageGenericFile, stageGenericHTML,
          stageMySpaceHTML, stageMySpaceOldXML,
          stageMySpacePlaylists, stageMySpaceArtistPlaylist,
          stageMySpaceSong, stageMySpaceMP3,
          stageYouTubeHTML
        } mStage; /*!< \brief \todo TODO */

   MagicQueue       *mpMagicQueue; /*!< \brief \todo TODO */
   Satellite        *mpSatellite; /*!< \brief \todo TODO */
   bool             mDownloadToFile; /*!< \brief \todo TODO */
   bool             mSuccess; /*!< \brief \todo TODO */
   QString          mContentType; /*!< \brief \todo TODO */
   QString          mReferer; /*!< \brief \todo TODO */
   QString          mMessage; /*!< \brief \todo TODO */
   QString          mFileName; /*!< \brief \todo TODO */
   QString          mBuffer; /*!< \brief \todo TODO */
   QFile            *mpFile; /*!< \brief \todo TODO */
   QBuffer          *mpBuffer; /*!< \brief \todo TODO */
   QString          mMySpaceArtId; /*!< \brief \todo TODO */
   QString          mMySpaceProfId; /*!< \brief \todo TODO */
   QString          mMySpacePlaylistId; /*!< \brief \todo TODO */
   QString          mMySpaceSongId; /*!< \brief \todo TODO */

   void downloadToFile();
   void downloadToBuffer();
   void downloadClose();
   void sanitizeFileName();

   QString xmlParam( const QString &string, int pos = 0 );

   void parseURL();
   void processGenericFile();
   void parseGenericHTML();
   void parseYouTubeHTML();
   void parseMySpaceHTML();
   void parseMySpaceOldXML();
   void parseMySpaceMP3();
   void parseMySpacePlaylists();
   void parseMySpaceArtistPlaylist();
   void parseMySpaceSong();
};

/*! @} */

#endif
