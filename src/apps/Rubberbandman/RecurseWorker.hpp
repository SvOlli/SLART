/**
 * src/apps/Rubberbandman/RecurseWorker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
#include <TagList.hpp>
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
 \brief TODO

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
          ModeNormTitle } mMode;

   bool                 mCancel;
   bool                 mSetArtist;
   bool                 mSetTitle;
   bool                 mSetAlbum;
   bool                 mSetYear;
   bool                 mSetGenre;
   bool                 mSetFlags;
   bool                 mUnsetFlags;
   bool                 mSetGroups;
   bool                 mUnsetGroups;
   QString              mDir;
   QString              mPatternWithTrackNr;
   QString              mPatternWithoutTrackNr;
   TrackInfo            mTrackInfo;
   TagList              mTagList;
   DatabaseInterface    *mpDatabase;
};

/*! @} */

#endif
