/*
 * src/apps/Partyman/PlayerWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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
#include <QProcess>

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


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Partyman/PlayerWidget_connect.dot" "Connect Graph"
*/
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
                 ControlWidget *controlWidget, Qt::WindowFlags flags = Qt::WindowFlags() );
   virtual ~PlayerWidget();

   /*!
    \brief get a new track from playlist

   */
   void getNextTrack( bool armed = true );
   /*!
    \brief get name of the currently loaded track

   */
   QString getCurrentTrack() const;
   /*!
    \brief update the time display

   */
   void updateTime( const QString &msg = QString(), bool force = false );
   /*!
    \brief set the track info like filename

   */
   void setInfo( const QString &line );
   /*!
    \brief updating the time played

   */
   void setPlayingStatus( const QString &line );
   /*!
    \brief set playing state

   */
   void setState( PlayerFSM::tState state );
   /*!
    \brief connect to DerMixD

   */
   void connectTo( const QString &hostname, int port );
   /*!
    \brief disconnect from DerMixD

   */
   void disconnect();
   /*!
    \brief skip current track

   */
   bool skip();
   /*!
    \brief report playing state

   */
   PlayerFSM::tState getState();
   /*!
    \brief toggle pause

   */
   void pause();
   /*!
    \brief toggle pause

   */
   void disablePlayPosition( bool disable );
   /*!
    \brief (re-)read configuration

   */
   void readConfig();
   /*!
    \brief disable slider while playing, if running in kiosk mode

   */
   void handleKioskMode( bool enable );
   /*!
    \brief update environment for notifier

   */
   void updateNotifier( const QString &name, const QString &value );
   /*!
    \brief update environment for notifier

   */
   void updateNotifier( const QProcessEnvironment &env );

public slots:
   /*!
    \brief lock seek slider from getting status updates

   */
   void lock();
   /*!
    \brief jump to a position in track

   */
   void seek();
   /*!
    \brief play next track due to special circumstances

    Used either when DerMixD is not reporting "stopped" or when
    "crossfade on next" is activated

   */
   void nextTrack();
   /*!
    \brief handle the change of the play position (pre-seek)

   */
   void playPosChange( int action );
   /*!
    \brief handle unload request

   */
   void unload( bool drop = false );
   /*!
    \brief handle successful connection to DerMixD

   */
   void handleConnect();
   /*!
    \brief handle disconnect from DerMixD

   */
   void handleDisconnect();
   /*!
    \brief handle responses from DerMixD

   */
   void handleResponse();
   /*!
    \brief handle notifier process

   */
   void handleNotifierDone( int exitCode, QProcess::ExitStatus exitStatus );
   /*!
    \brief handle DerMixD connection errors

   */
   void handleError( QAbstractSocket::SocketError socketError );

signals:
   /*!
    \brief signal the track playing on start/resume

   */
   void trackPlaying( const TrackInfo &trackInfo );

protected:
   /*!
    \brief reimplemented for the drop of drag'n'drop of tracks

   */
   void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief reimplemented for the drop of drag'n'drop of tracks

   */
   void dropEvent( QDropEvent *event );

private:
   enum eDerMixD { inNormal, inFullstat, inScan };

   PlayerWidget( const PlayerWidget &that );
   PlayerWidget &operator=( const PlayerWidget &that );

   /*!
    \brief send a command to DerMixD

   */
   void sendCommand( const QString &command, const QString &parameter = QString() );
   /*!
    \brief enable/disable watching of track status

   */
   void watch( bool turnWatchOn );
   /*!
    \brief adjust volume for normalization

   */
   void handleScan( const QString &data );
   /*!
    \brief set the volume according to peak or power

   */
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
   QProcess      *mpNotifier;
   bool          mStartOther;
   bool          mAutoPlay;
   bool          mConsole;
   bool          mPlayPosMoved;
   long          mTotalTime;
   long          mEndTime;
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

/*! @} */

#endif
