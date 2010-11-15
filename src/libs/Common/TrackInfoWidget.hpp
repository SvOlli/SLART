/**
 * src/libs/Common/TrackInfoWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACKINFOWIDGET_HPP
#define TRACKINFOWIDGET_HPP TRACKINFOWIDGET_HPP

#include <QWidget>
#include <QString>

#include "TrackInfo.hpp"

class QCheckBox;
class QLabel;
class QPushButton;

class Database;
class ScrollLine;


class TrackInfoWidget : public QWidget
{
Q_OBJECT
   
public:
   TrackInfoWidget( Database *database, const QByteArray &updateCode,
                    bool includeFolders, QWidget *parent = 0 );
   
public slots:
   /* handle the "Favorite" checkbox */
   void handleFavoriteButton();
   /* handle the "No Auto" checkbox */
   void handleUnwantedButton();
   /* remotely set both "Favorite" and "No Auto" checkboxes */
   void setFavoriteUnwanted( bool favorite, bool unwanted );
   /* update the track data */
   void getTrack( const TrackInfo &trackInfo );
   /* update the displayed track info */
   void update( bool reread = true );
   
signals:
   /* either "Favorite" or "No Auto" checkbox has been clicked */
   void checkboxClicked( const TrackInfo &trackInfo );
   
private:
   TrackInfoWidget( const TrackInfoWidget &that );
   TrackInfoWidget &operator=( const TrackInfoWidget &that );
   
   Database    *mpDatabase;
   TrackInfo   mTrackInfo;
   QByteArray  mUpdateCode;
   bool        mIncludeFolders;
   QLabel      *mpTimesPlayed;
   ScrollLine  *mpArtist;
   ScrollLine  *mpTitle;
   ScrollLine  *mpAlbum;
   ScrollLine  *mpTrackNr;
   QCheckBox   *mpFavoriteButton;
   QCheckBox   *mpUnwantedButton;
};

#endif
