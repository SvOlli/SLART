/**
 * TrackInfoWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
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
   TrackInfoWidget( Database *database, const QString &updateCode,
                    QWidget *parent = 0 );
   
public slots:
   /* handle the "Favorite" checkbox */
   void handleFavoriteButton();
   /* handle the "No Auto" checkbox */
   void handleUnwantedButton();
   /* update the track data */
   void getTrack( const TrackInfo &trackInfo );
   /* update the displayed track info */
   void update( bool reread = true );
   
signals:
   
private:
   TrackInfoWidget( const TrackInfoWidget &other );
   TrackInfoWidget &operator=( const TrackInfoWidget &other );
   
   Database    *mpDatabase;
   TrackInfo   mTrackInfo;
   QString     mUpdateCode;
   QLabel      *mpTimesPlayed;
   ScrollLine  *mpArtist;
   ScrollLine  *mpTitle;
   ScrollLine  *mpAlbum;
   ScrollLine  *mpTrackNr;
   QCheckBox   *mpFavoriteButton;
   QCheckBox   *mpUnwantedButton;
};

#endif
