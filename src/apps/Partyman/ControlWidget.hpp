/*
 * src/apps/Partyman/ControlWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONTROLWIDGET_HPP
#define CONTROLWIDGET_HPP CONTROLWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */
//#include <stdio.h>

/* Qt headers */
#include <QIcon>
#include <QProcess>
#include <QString>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QUdpSocket>

/* local library headers */

/* local headers */
#include "PlayerWidget.hpp"

/* forward declaration of Qt classes */
class QPushButton;
class QMenu;
class QAction;

/* forward declaration of local classes */
class ConfigDialog;
class Database;
class GenericSatMsgHandler;
class PlaylistControlWidget;
class Satellite;
class TrackInfo;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief TODO

*/
class ControlWidget : public QWidget
{
   Q_OBJECT
   Q_ENUMS( ErrorCode )

public:
   enum ErrorCode
   {
      ErrorNone,
      ErrorNoConnection,
      ErrorConnectionLost,
      ErrorWrongVersion
   };

   ControlWidget( Database *database, ConfigDialog *config,
                  PlaylistControlWidget *playlist, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~ControlWidget();

   /*!
    \brief callback for player to request next track

   */
   void getNextTrack( QString *fileName );
   /*!
    \brief change the FSM state of the other player

   */
   void changeOtherState( int player, PlayerFSM::tState state );
   /*!
    \brief add files to playlist (for unloading)

   */
   void addToPlaylist( const QStringList &entries );
   /*!
    \brief run SLARTCommunication & execute external logger

   */
   void log( const QString &udpEvent, const QString &logEvent, const QString &data = QString() );
   /*!
    \brief signalise that skipping is possible again

   */
   void allowSkip();
   /*!
    \brief check if skip button is locked in

   */
   bool isSkipChecked();
   /*!
    \brief save the current tracks for restart

   */
   void saveTracks( bool unload );

private:

public slots:
   /*!
    \brief (re-)read configuration

   */
   void readConfig();
   /*!
    \brief only allow connect if a valid database is provided

   */
   void allowConnect( bool allowed );
   /*!
    \brief handle connect to DerMixD

   */
   void initConnect();
   /*!
    \brief handle disconnect from DerMixD

   */
   void initDisconnect( ErrorCode errorCode = ErrorNone );
   /*!
    \brief skip to next track

   */
   void handleSkipTrack();
   /*!
    \brief handle (un)pausing

   */
   void handlePause( bool reset = false );
   /*!
    \brief handle load from clipboard

   */
   void handleLoad();
   /*!
    \brief handle remote requests

   */
   void handleSatellite( const QByteArray &msg );
   /*!
    \brief handle process startup

   */
   void handleDerMixDstartup();
   /*!
    \brief handle process finish

   */
   void handleDerMixDfinish( int exitCode, QProcess::ExitStatus exitStatus );
   /*!
    \brief handle process error

   */
   void handleDerMixDerror( QProcess::ProcessError error );
   /*!
    \brief handle the info of the currently played track

   */
   void handleTrackPlaying( const TrackInfo &trackInfo );
   /*!
    \brief handle the click on the tray icon

   */
   void handleTrayIcon( QSystemTrayIcon::ActivationReason reason );
   /*!
    \brief disable some elements of the user interface, if running in kiosk mode

   */
   void handleKioskMode( bool enable );

signals:
   /*!
    \brief request a new icon and title

   */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   /*!
    \brief request for adding file to playlist (used for unload)

   */
   void requestAddToPlaylist( const QStringList &entries, bool atStart );
   /*!
    \brief signal if a connection has been established or cut off

   */
   void signalConnected( bool connected );
   /*!
    \brief signal to handle a "k0u" and "r0u" SLART message

   */
   void trackUpdate();
   /*!
    \brief request to change the tab

   */
   void requestTab( int tabNr );

private:
   ControlWidget( const ControlWidget &that );
   ControlWidget &operator=( const ControlWidget &that );

   ConfigDialog            *mpConfig;
   PlaylistControlWidget   *mpPlaylist;
   Satellite               *mpSatellite;
   GenericSatMsgHandler    *mpGenericSatMsgHandler;
   QPushButton             *mpSettingsButton;
   QPushButton             *mpConnectButton;
   QPushButton             *mpSkipButton;
   PlayerWidget            *mpPlayer[2];
   bool                    mConnected;
   bool                    mPaused;
   QIcon                   mPartymanIcon;
   QIcon                   mStopIcon;
   QIcon                   mPlayIcon;
   QIcon                   mPauseIcon;
   QIcon                   mSkipIcon;
   QIcon                   mLoadIcon;
   QSystemTrayIcon         *mpTrayIcon;
   QMenu                   *mpTrayIconStopMenu;
   QMenu                   *mpTrayIconPlayMenu;
   QMenu                   *mpDisconnectMenu;
   QAction                 *mpPlayAction;
   QAction                 *mpSkipAction;
   QAction                 *mpPauseAction;
   QAction                 *mpDisconnectAction;
   QAction                 *mpLoadAction;
   bool                    mKioskMode;
   QTimer                  mTrayIconClickTimer;
   QProcess                mDerMixDprocess;
   QProcess                mLoggerProcess;
   bool                    mWaitForDerMixD;
   bool                    mDerMixDstarted;
   QByteArray              mLastP0p;
};

/*! @} */

#endif
