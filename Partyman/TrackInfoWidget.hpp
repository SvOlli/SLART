/**
 * TrackInfoWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef TRACKINFOWIDGET_HPP
#define TRACKINFOWIDGET_HPP TRACKINFOWIDGET_HPP

#include <QWidget>

#include "TrackInfo.hpp"

class QLabel;
class QPushButton;
class Database;

class ScrollLine;

class TrackInfoWidget : public QWidget
{
Q_OBJECT

public:
   TrackInfoWidget( Database *database, QWidget *parent = 0 );

public slots:
   void handleFavoriteButton();
   void handleUnwantedButton();
   void getTrack( const QString &fileName );

signals:

private:
   TrackInfoWidget( const TrackInfoWidget &other );
   TrackInfoWidget &operator=( const TrackInfoWidget &other );

   Database    *mpDatabase;
   TrackInfo   mTrackInfo;
   QLabel      *mpArtistLabel;
   QLabel      *mpTitleLabel;
   QLabel      *mpAlbumLabel;
   ScrollLine  *mpArtist;
   ScrollLine  *mpTitle;
   ScrollLine  *mpAlbum;
   QPushButton *mpFavoriteButton;
   QPushButton *mpUnwantedButton;
};

#endif
