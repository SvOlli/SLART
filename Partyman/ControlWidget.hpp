/**
 * ControlWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONTROLWIDGET_HPP
#define CONTROLWIDGET_HPP CONTROLWIDGET_HPP

#include <QWidget>

#include <QIcon>
#include <QProcess>
#include <QString>
#include <QUdpSocket>

#include "PlayerWidget.hpp"
#include "SLARTCom.hpp"

class QPushButton;
class QMenu;
class QAction;
   
class ConfigDialog;
class Database;
class PlaylistWidget;
class TrackInfo;


class ControlWidget : public QWidget
{
Q_OBJECT
public:
   enum eErrorCode
   { 
      noError, 
      noConnection, 
      connectionLost, 
      wrongVersion
   };
   
   ControlWidget( Database *database, ConfigDialog *config,
                  PlaylistWidget *playlist, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~ControlWidget();
   
   /* callback for player to request next track */
   void getNextTrack( QString *fileName );
   /* change the FSM state of the other player */
   void changeOtherState( int player, PlayerFSM::tState state );
   /* add files to playlist (for unloading) */
   void addToPlaylist( const QStringList &entries );
   /* (de)activate Skip/Next button and time sliders */
   void allowInteractive( bool allow );
   /* run SLARTCommunication & execute external logger */
   void log( const QString &udpEvent, const QString &logEvent, const QString &data = QString() );
   
private:
   /* save the current tracks for restart */
   void saveTracks();
   
public slots:
   /* (re-)read configuration */
   void readConfig();
   /* only allow connect if a valid database is provided */
   void allowConnect( bool allowed );
   /* handle connect to DerMixD */
   void initConnect();
   /* handle disconnect from DerMixD */
   void initDisconnect( eErrorCode errorCode = noError );
   /* skip to next track */
   void handleSkipTrack();
   /* handle (un)pausing */
   void handlePause( bool reset = false );
   /* handle load from clipboard */
   void handleLoad();
   /* handle remote requests */
   void handleSLART( const QStringList &package );
   /* handle process startup */
   void handleDerMixDstartup();
   /* handle process finish */
   void handleDerMixDfinish( int exitCode, QProcess::ExitStatus exitStatus );
   /* handle process error */
   void handleDerMixDerror( QProcess::ProcessError error );
   /* handle the info of the currently played track */
   void handleTrackPlaying( const TrackInfo &trackInfo );
   
signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   /* request for adding file to playlist (used for unload) */
   void requestAddToPlaylist( const QStringList &entries, bool atStart );
   /* signal if a connection has been established or cut off */
   void signalConnected( bool connected );
   /* signal to handle a "k0u" and "r0u" SLART message */
   void trackUpdate();
   
private:
   ControlWidget( const ControlWidget &other );
   ControlWidget &operator=( const ControlWidget &other );
   
   ConfigDialog   *mpConfig;
   PlaylistWidget *mpPlaylist;
   QLabel         *mpLogo;
   QPushButton    *mpConnectButton;
   QPushButton    *mpSkipButton;
   PlayerWidget   *mpPlayer[2];
   bool           mConnected;
   bool           mPaused;
   QIcon          mStopIcon;
   QIcon          mPlayIcon;
   QIcon          mPauseIcon;
   QIcon          mLoadIcon;
   QMenu          *mpDisconnectMenu;
   QAction        *mpPauseAction;
   QAction        *mpDisconnectAction;
   QAction        *mpLoadAction;
   SLARTCom       mSLARTCom;
   QProcess       mDerMixDprocess;
   QProcess       mLoggerProcess;
   bool           mWaitForDerMixD;
   bool           mDerMixDstarted;
   QString        mLastTitle;
   QString        mLastP0p;
};

#endif
