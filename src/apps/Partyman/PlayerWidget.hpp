/**
 * src/apps/Partyman/PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLAYERWIDGET_HPP
#define PLAYERWIDGET_HPP PLAYERWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QMutex>
#include <QString>

/* local library headers */
#include "TrackInfo.hpp"

/* local headers */
#include "PlayerFSM.hpp"

/* forward declaration of Qt classes */
class QLabel;
class QLineEdit;
class QScrollArea;
class QSlider;
class QTcpSocket;

/* forward declaration of local classes */
class ControlWidget;
class Database;
class ScrollLine;
class TimeSlider;


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
   bool skip();
   /* report playing state */
   PlayerFSM::tState getState() { return mpFSM->getState(); };
   /* toggle pause */
   void pause();
   /* toggle pause */
   void disablePlayPosition( bool disable );
   /* (re-)read configuration */
   void readConfig();
   /* disable slider while playing, if running in kiosk mode */
   void handleKioskMode( bool enable );
   
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
   /* signal the track playing on start/resume */
   void trackPlaying( const TrackInfo &trackInfo );

protected:
   /*  */
   void dragEnterEvent( QDragEnterEvent *event );
   /*  */
   void dropEvent( QDropEvent *event );
   
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
   /* set the volume according to peak or power */
   bool setVolume();
   
   int           mPlayer;
   Database      *mpDatabase;
   ControlWidget *mpControlWidget;
   ScrollLine    *mpScrollLine;
   QLabel        *mpStatusDisplay;
   QLabel        *mpTimeDisplay;
   TimeSlider    *mpPlayPosition;
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
   bool          mKioskMode;
   int           mNormalizeMode;
   double        mNormalizeValue;
   QString       mDisplayPattern;
   TrackInfo     mTrackInfo;
};

#endif
