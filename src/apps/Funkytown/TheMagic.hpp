/**
 * src/apps/Funkytown/TheMagic.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef THEMAGIC_HPP
#define THEMAGIC_HPP THEMAGIC_HPP

#include <QStringList>
#include <QHttp>

class QBuffer;
class QFile;
class QListWidget;
class QListWidgetItem;

class MagicQueue;
class Satellite;

class TheMagic : public QObject
{
Q_OBJECT
   
   friend class MagicQueue;
   
public:
   TheMagic( MagicQueue *magicQueue = 0 );
   virtual ~TheMagic();
   
   TheMagic( const TheMagic &other );

   /* the magic that gonna happen before the download */
   void preDownload();
   /* the magic that gonna happen after the download */
   void postDownload( bool success );
   /* return filename for the file that being downloaded, if any */
   QString fileName();
   /* return referer, if any */
   QString referer();
   /* note that the download has failed */
   void fail();
   /* sset the content (mime-)type, used for 8bit conversion */
   void setContentType( const QString &contentType );
   /* the device handler to write the data to */
   QIODevice *ioDevice();
   
   /* the url to download */
   QString          mURL;
   /* marker if the list entry has been selected (for sending to Partyman) */
   bool             mSelected;
   
   
private:
   TheMagic &operator=( const TheMagic &other );

   /* the different states of downloading and analysing */
   enum { stageFresh, stageGenericFile, stageGenericHTML,
          stageMySpaceHTML, stageMySpaceOldXML,
          stageMySpacePlaylists, stageMySpaceArtistPlaylist,
          stageMySpaceSong, stageMySpaceMP3,
          stageYouTubeHTML
        } mStage;
   
   MagicQueue       *mpMagicQueue;
   Satellite        *mpSatellite;
   bool             mDownloadToFile;
   bool             mSuccess;
   QString          mContentType;
   QString          mReferer;
   QString          mMessage;
   QString          mFileName;
   QString          mBuffer;
   QFile            *mpFile;
   QBuffer          *mpBuffer;
   QString          mMySpaceArtId;
   QString          mMySpaceProfId;
   QString          mMySpacePlaylistId;
   QString          mMySpaceSongId;
   
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

#endif
