/**
 * PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PLAYERWIDGET_HPP
#define PLAYERWIDGET_HPP PLAYERWIDGET_HPP

#include <QWidget>
#include <QMutex>
#include <QString>


class QTcpSocket;
class QLineEdit;
class QLabel;
class QScrollArea;
class QSlider;
class ControlWidget;
class ScrollLine;
class Database;
#include "PlayerFSM.hpp"
#include "TrackInfo.hpp"


class PlayerWidget : public QWidget
{
Q_OBJECT

   friend class PlayerFSMDisconnected;
   friend class PlayerFSMSearching;
   friend class PlayerFSMLoading;
   friend class PlayerFSMReady;
   friend class PlayerFSMPlaying;
   friend class PlayerFSMEnding;
   friend class PlayerFSMSeeking;
   friend class PlayerFSMPaused;
   friend class PlayerFSMEndingPaused;
   
public:
   PlayerWidget( int index, Database *database, 
                 ControlWidget *controlWidget, Qt::WindowFlags flags = 0 );
   virtual ~PlayerWidget();

   /* get a new track from playlist */
   void getNextTrack( bool armed = true );
   /* get name of the currently loaded track */
   QString getCurrentTrack() const;
   /* update the time display */
   void updateTime( const QString &msg = QString(), bool force = false );
   /* set the track info like filename */
   void setInfo( const QString &line );
   /* updating the time played */
   void setPlayingStatus( const QString &line );
   /* set playing state */
   void setState( PlayerFSM::tState state );
   /* connect to DerMixD */
   void connectTo( const QString &hostname, int port );
   /* disconnect from DerMixD */
   void disconnect();
   /* skip current track */
   void skip();
   /* report playing state */
   PlayerFSM::tState getState() { return mpFSM->getState(); };
   /* toggle pause */
   void pause();
   /* toggle pause */
   void disablePlayPosition( bool disable );
   /* (re-)read configuration */
   void readConfig();
   /* (reread current track information */
   void rereadTrackInfo();

public slots:
   /* lock seek slider from getting status updates */
   void lock();
   /* jump to a position in track */
   void seek();
   /* handle the change of the play position (pre-seek) */
   void playPosChange( int action );
   /* handle unload request */
   void unload();
   /* handle successful connection to DerMixD */
   void handleConnect();
   /* handle disconnect from DerMixD */
   void handleDisconnect();
   /* handle responses from DerMixD */
   void handleResponse();
   /* handle DerMixD connection errors */
   void handleError( QAbstractSocket::SocketError socketError );

signals:
   void trackPlaying( const TrackInfo &trackInfo );

private:
   enum eDerMixD { inNormal, inFullstat, inScan };
   
   PlayerWidget( const PlayerWidget &other );
   PlayerWidget &operator=( const PlayerWidget &other );
   
   /* send a command to DerMixD */
   void sendCommand( const QString &command, const QString &parameter = QString() );
   /* enable/disable watching of track status */
   void watch( bool turnWatchOn );
   /* adjust volume for normalization */
   void handleScan( const QString &data );
   /*  */
   bool setVolume();

   int           mPlayer;
   Database      *mpDatabase;
   ControlWidget *mpControlWidget;
   ScrollLine    *mpScrollLine;
   QLabel        *mpStatusDisplay;
   QLabel        *mpTimeDisplay;
   QSlider       *mpPlayPosition;
   QTcpSocket    *mpSocket;
   PlayerFSM     *mpFSM;
   bool          mStartOther;
   bool          mAutoPlay;
   bool          mConsole;
   bool          mPlayPosMoved;
   long          mTotalTime;
   long          mFrequency;
   long          mSamples;
   long          mHeadStart;
   bool          mUpdateSlider;
   int           mNormalizeMode;
   double        mNormalizeValue;
   QString       mDisplayPattern;
   TrackInfo     mTrackInfo;
};

#endif
