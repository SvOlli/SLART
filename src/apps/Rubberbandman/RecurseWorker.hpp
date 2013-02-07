/*
 * src/apps/Rubberbandman/RecurseWorker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef RECURSEWORKER_HPP
#define RECURSEWORKER_HPP RECURSEWORKER_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */
#include <TagMap.hpp>
#include <TrackInfo.hpp>
#include <DirWalker.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;

/* forward declaration of local classes */
class DatabaseInterface;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class RecurseWorker : public QThread
{
   Q_OBJECT

public:
   enum Flag {
      SetFlags     = 0x01,
      UnsetFlags   = 0x02,
      SetGroups    = 0x04,
      UnsetGroups  = 0x08
   };
   Q_DECLARE_FLAGS(Flags, Flag)

   RecurseWorker();
   virtual ~RecurseWorker();

   /*!
    \brief 

   */
   void startSetTags( const QString &dir, const TrackInfo &ti, Flags flags );
   /*!
    \brief 

   */
   void startNormArtist( const QString &dir );
   /*!
    \brief 

   */
   void startNormTitle( const QString &dir );

public slots:
   /*!
    \brief 

   */
   void updateTrackInfo( const TrackInfo &trackInfo );
   /*!
    \brief final call of a batch to stop event loop

   */
   void done();

signals:
   /*!
    \brief emit some kind of progress

   */
   void currentPath( const QString &path );
   /*!
    \brief send out if there are errors

    \param message error message
    \param fileName name of file the error occured on
    */
   void error( const QString &message, const QString &fileName );

private:
   Q_DISABLE_COPY( RecurseWorker )

   /*!
    \brief run the job

   */
   void run();

   /*!
    \brief 

   */
   void workDir( const QString &dir );

   /*!
    \brief mode of operation

   */
   enum { ModeNothing,
          ModeSetTags,
          ModeNormArtist,
          ModeNormTitle } mMode;  /*!< \brief mode of operation */

   bool                 mCancel; /*!< \brief cancelation indicator */
   bool                 mSetArtist; /*!< \brief set the artist */
   bool                 mSetTitle; /*!< \brief set the track title */
   bool                 mSetAlbum; /*!< \brief set the album title */
   bool                 mSetYear; /*!< \brief set the year */
   bool                 mSetGenre; /*!< \brief set the genre */
   bool                 mSetFlags; /*!< \brief set flags */
   bool                 mUnsetFlags; /*!< \brief unset flags */
   bool                 mSetGroups; /*!< \brief add to groups */
   bool                 mUnsetGroups; /*!< \brief remove from groups */
   QString              mDir;/*!< \brief \todo complete documentation */
   QString              mPatternWithTrackNr;/*!< \brief \todo complete documentation */
   QString              mPatternWithoutTrackNr;/*!< \brief \todo complete documentation */
   TrackInfo            mTrackInfo;/*!< \brief \todo complete documentation */
   TagMap               mTagMap;/*!< \brief \todo complete documentation */
   DatabaseInterface    *mpDatabase;/*!< \brief \todo complete documentation */
};

/*! @} */

#endif
