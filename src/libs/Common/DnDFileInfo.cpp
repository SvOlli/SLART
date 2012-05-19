/*
 * src/libs/Common/DnDFileInfo.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "DnDFileInfo.hpp"

/* system headers */

/* Qt headers */
#include <QLabel>
#include <QMimeData>
#include <QUrl>
#include <QDrag>
#include <QToolTip>
#include <QStyle>

/* local library headers */

/* local headers */
#include "TrackInfo.hpp"

/* class variables */


DnDFileInfo::DnDFileInfo( QWidget *parent )
: QObject( parent )
, mpParent( parent )
, mDatabaseID(0)
, mFileInfo()
, mArtist()
, mTitle()
{
}


DnDFileInfo::DnDFileInfo( quint32 databaseID, const QFileInfo &fileInfo,
                          const QString &artist, const QString &title, QWidget *parent )
: QObject( parent )
, mpParent( parent )
, mDatabaseID( databaseID )
, mFileInfo( fileInfo )
, mArtist( artist )
, mTitle( title )
{
}


DnDFileInfo::DnDFileInfo(const TrackInfo &trackInfo, QWidget *parent)
: QObject( parent )
, mpParent( parent )
, mDatabaseID( trackInfo.mID )
, mFileInfo( trackInfo.filePath() )
, mArtist( trackInfo.mArtist )
, mTitle( trackInfo.mTitle )
{
}


void DnDFileInfo::clear()
{
   mDatabaseID = 0;
   mFileInfo = QFileInfo();
   mArtist.clear();
   mTitle.clear();
}


void DnDFileInfo::setTrackInfo(const TrackInfo &trackInfo)
{
   mDatabaseID = trackInfo.mID;
   mFileInfo = QFileInfo( trackInfo.filePath() );
   mArtist = trackInfo.mArtist;
   mTitle = trackInfo.mTitle;
}


QString DnDFileInfo::filePath()
{
   return mFileInfo.absoluteFilePath();
}


void DnDFileInfo::setFilePath(const QString &filePath)
{
   mDatabaseID = 0;
   mFileInfo = QFileInfo( filePath );
   mArtist.clear();
   mTitle.clear();
}


QMimeData *DnDFileInfo::createMimeData()
{
   QMimeData *mimeData = new QMimeData;
   mimeData->setText( mFileInfo.absoluteFilePath() );
   QList<QUrl> urls;
   urls << QUrl::fromLocalFile( mFileInfo.absoluteFilePath() );
   mimeData->setUrls( urls );

   return mimeData;
}


QDrag *DnDFileInfo::createDrag()
{
   /* partially taken from QToolTip */
   QLabel w;
   w.ensurePolished();
   w.setMargin( 1 + w.style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth, 0, &w) );
   w.setFrameStyle( QFrame::Panel );
   w.setAlignment( Qt::AlignLeft );
   w.setIndent( 1 );
   w.setWindowOpacity( w.style()->styleHint(QStyle::SH_ToolTipLabel_Opacity, 0, &w) / qreal(255.0) );
   w.setWordWrap( false );
   QStringList textBlock;
   textBlock << mFileInfo.absolutePath()
             << mFileInfo.fileName()
             << mArtist << mTitle ;
   w.setText( textBlock.join("\n") );
   QFontMetrics fm(w.font());
   QSize extra(1, 0);
   if (fm.descent() == 2 && fm.ascent() >= 11)
   {
      ++extra.rheight();
   }
   w.resize(w.sizeHint() + extra);

   QDrag *drag = new QDrag( mpParent );
   drag->setMimeData( createMimeData() );
   drag->setPixmap( QPixmap::grabWidget( &w ) );
   drag->setHotSpot( QPoint( w.width() / 2, w.height() / 2 ) );

   return drag;
}
