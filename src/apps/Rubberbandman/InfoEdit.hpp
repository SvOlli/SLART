/*
 * src/apps/Rubberbandman/InfoEdit.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef INFOEDIT_HPP
#define INFOEDIT_HPP INFOEDIT_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */
#include <TagList.hpp>
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QCheckBox;
class QComboBox;
class QDir;
class QGroupBox;
class QIntValidator;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;

/* forward declaration of local classes */
class DatabaseInterface;
class ScrollLine;
class LineEdit;
class RecurseWorker;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Rubberbandman/InfoEdit_connect.dot" "Connect Graph"
*/
class InfoEdit : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   InfoEdit( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~InfoEdit();

   /*!
    \brief \todo complete documentation

   */
   QString tagsFileName( const QString &pattern, bool filterPath = true );
   /*!
    \brief \todo complete documentation

   */
   QString fileName();

   /*!
    \brief add the current entry to the completer

   */
   static void addToCompleter( QLineEdit *lineEdit );

public slots:
   /*!
    \brief load tags from a file

   */
   void load( const QString &fullpath = QString() );

   /*!
    \brief \todo complete documentation

   */
   void loadTrackInfo( const TrackInfo &trackInfo );

   /*!
    \brief handle save button

   */
   void handleSetSave();

   /*!
    \brief handle norm. artist button

   */
   void handleNormalizeArtist();

   /*!
    \brief handle norm. title button

   */
   void handleNormalizeTitle();

   /*!
    \brief handle cancel button

   */
   void handleCancel();

   /*!
    \brief enable save button

   */
   void handleChange();

   /*!
    \brief handle the flags menu

   */
   void handleFlagsMenu( QAction *action );

   /*!
    \brief handle the folders menu

   */
   void handleFoldersMenu( QAction *action );

   /*!
    \brief \todo complete documentation

   */
   void handleFoldersEntries( const QStringList &folders );

signals:
   /*!
    \brief data has been read

   */
   void fileStats( bool isValid, bool isFile );

   /*!
    \brief the data has been updated

   */
   void updated();

private:
   Q_DISABLE_COPY( InfoEdit )

   /*!
    \brief normalize a line edit field

   */
   void normalize( QLineEdit *lineEdit );

   /*!
    \brief load tags from a file

   */
   void loadFile( const QString &fullpath );

   /*!
    \brief save the currently edited tags

   */
   void saveFile();

   /*!
    \brief update the info of the track concerning flags and folders

   */
   void updateDatabaseInfo( bool withRecurse );

   DatabaseInterface    *mpDatabase;
   RecurseWorker        *mpRecurseWorker;
   TrackInfo            mTrackInfo;

   QPushButton          *mpButtonSet;
   QPushButton          *mpButtonNormArtist;
   QPushButton          *mpButtonNormTitle;
   QPushButton          *mpButtonCancel;

   QGroupBox            *mpFileGroupBox;
   QGroupBox            *mpTagGroupBox;
   QGroupBox            *mpDatabaseGroupBox;

   ScrollLine           *mpShowPathName;
   ScrollLine           *mpShowFileName;
   ScrollLine           *mpShowSize;
   ScrollLine           *mpShowPlayTime;

   LineEdit             *mpEditArtist;
   LineEdit             *mpEditTitle;
   LineEdit             *mpEditAlbum;
   QLineEdit            *mpEditTrackNr;
   QLineEdit            *mpEditYear;
   LineEdit             *mpEditGenre;
   QIntValidator        *mpValidateTrackNr;
   QIntValidator        *mpValidateYear;

   QPushButton          *mpButtonFlags;
   QMenu                *mpMenuFlags;
   QLabel               *mpShowTimesPlayed;
   QPushButton          *mpButtonFolders;
   QMenu                *mpMenuFolders;
   QAction              *mpRecurseSetFlags;
   QAction              *mpRecurseUnsetFlags;
   QAction              *mpFavoriteTrackFlag;
   QAction              *mpUnwantedTrackFlag;
   QAction              *mpTrackScannedFlag;
   QAction              *mpRecurseSetFolders;
   QAction              *mpRecurseUnsetFolders;

   bool                 mIsValid;
   bool                 mIsFile;
   bool                 mCancel;
   QString              mFileName;
};

/*! @} */

#endif
