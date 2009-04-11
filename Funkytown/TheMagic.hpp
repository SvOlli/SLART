/**
 * TheMagic.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef THEMAGIC_HPP
#define THEMAGIC_HPP THEMAGIC_HPP

#include <QStringList>
#include <QHttp>

class MagicQueue;

class QBuffer;
class QFile;
class QListWidget;
class QListWidgetItem;


class TheMagic : public QObject
{
Q_OBJECT
   
   friend class MagicQueue;
   
public:
   TheMagic( MagicQueue *magicQueue = 0 );
   virtual ~TheMagic();
   
   TheMagic( const TheMagic &other );
   TheMagic &operator=( const TheMagic &other );
   bool operator==( const TheMagic &other ) const;
   
   /*  */
   QListWidgetItem *listWidgetItem();
   /*  */
   void preDownload();
   /*  */
   void postDownload( bool success );
   /*  */
   QIODevice *ioDevice();
   /*  */
   const QString &message();

   /*  */
   QString fileName();
   
   /*  */
   QString          mURL;
   
   
private:
   /*  */
   enum { stageFresh, stageGenericFile, stageGenericHTML,
          stageMySpaceHTML, stageMySpaceOldXML,
          stageMySpacePlaylists, stageMySpaceArtistPlaylist,
          stageMySpaceSong, stageMySpaceMP3,
          stageYouTubeHTML
        } mStage;
   
   MagicQueue       *mpMagicQueue;
   bool             mDownloadToFile;
   bool             mSuccess;
   QString          mMessage;
   QString          mFileName;
   QString          mBuffer;
   QFile            *mpFile;
   QBuffer          *mpBuffer;
   QListWidget      *mpListWidget;
   QListWidgetItem  *mpListWidgetItem;
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
