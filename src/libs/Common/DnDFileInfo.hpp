/*
 * src/libs/Common/DnDFileInfo.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DNDFILEINFO_HPP
#define DNDFILEINFO_HPP DNDFILEINFO_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QFileInfo>
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDrag;
class QMimeData;
class QWidget;

/* forward declaration of local classes */
class TrackInfo;

/*!
  \addtogroup Common

  @{
*/

/*!
 \brief hold file information and provides methods for nice drag'n'drop support.

 DnDFileInfo is intended to capsulate most of the stuff needed for drag'n'drop.
 This includes generating a bitmap for the draggable object and mime handling.
*/
 /*\dotfile "graphs/libs/Common/DnDFileInfo_connect.dot" "Connect Graph"*/
class DnDFileInfo : public QObject
{
public:

   /*!
    \brief default constructor

    create an object with all empty elements
    \param parent
   */
   DnDFileInfo( QWidget *parent = 0 );

   /*!
    \brief easy constructor

    create an object with all elements filled (may be obsolete)
    \param databaseID
    \param fileInfo
    \param artist
    \param title
    \param parent
   */
   DnDFileInfo( quint32 databaseID, const QFileInfo &fileInfo,
                const QString &artist, const QString &title, QWidget *parent = 0 );

   /*!
    \brief constructor using trackInfo

    \param trackInfo
    \param parent
   */
   DnDFileInfo( const TrackInfo &trackInfo, QWidget *parent = 0 );

   /*!
    \brief clear all data (may be obsolete)

   */
   void clear();

   /*!
    \brief property that holds path and filename of track
    */
   Q_PROPERTY(QString filePath
              READ filePath
              WRITE setFilePath)
   /*!
    \brief get filePath

   */
   QString filePath();

   /*!
    \brief set filePath

    \param filePath
   */
   void setFilePath( const QString &filePath );

   /*!
    \brief generate mime infomation

   */
   QMimeData *createMimeData();

   /*!
    \brief generate drag element

   */
   QDrag *createDrag();

public slots:
   /*!
    \brief set data from TrackInfo

    \param trackInfo
   */
   void setTrackInfo( const TrackInfo &trackInfo );

private:
   Q_DISABLE_COPY( DnDFileInfo )

   QWidget     *mpParent;
   quint32     mDatabaseID;
   QFileInfo   mFileInfo;
   QString     mArtist;
   QString     mTitle;
};

/*! @} */

#endif
