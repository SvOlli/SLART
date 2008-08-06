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

class QLabel;
class QPushButton;
class QCheckBox;
class Database;

class ScrollLine;

class TrackInfoWidget : public QWidget
{
Q_OBJECT

public:
   TrackInfoWidget( Database *database, const QString &updateCode,
                    QWidget *parent = 0 );

public slots:
   /*  */
   void handleFavoriteButton();
   /*  */
   void handleUnwantedButton();
   /*  */
   void getTrack( const TrackInfo &trackInfo );
   /*  */
   void update( bool reread = true );

signals:

private:
   TrackInfoWidget( const TrackInfoWidget &other );
   TrackInfoWidget &operator=( const TrackInfoWidget &other );

   Database    *mpDatabase;
   TrackInfo   mTrackInfo;
   QString     mUpdateCode;
   QLabel      *mpArtistLabel;
   QLabel      *mpTitleLabel;
   QLabel      *mpAlbumLabel;
   QLabel      *mpTrackNrLabel;
   QLabel      *mpTimesPlayed;
   ScrollLine  *mpArtist;
   ScrollLine  *mpTitle;
   ScrollLine  *mpAlbum;
   ScrollLine  *mpTrackNr;
   QCheckBox   *mpFavoriteButton;
   QCheckBox   *mpUnwantedButton;
};

#endif
