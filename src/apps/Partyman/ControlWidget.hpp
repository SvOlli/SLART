/*
 * src/apps/Partyman/ControlWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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
class QAction;
class QMenu;
class QPushButton;
class QToolButton;

/* forward declaration of local classes */
class PartymanConfigDialog;
class Database;
class GenericSatelliteHandler;
class PartymanMainWindow;
class Satellite;
class TrackInfo;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Partyman/ControlWidget_connect.dot" "Connect Graph"
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

   ControlWidget( Database *database, PartymanConfigDialog *config,
                  PartymanMainWindow *parent = 0 );
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
   void requestTab( const QString &name );

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
   Q_DISABLE_COPY( ControlWidget )

   /*!
    \brief set appropiate set of actions according to current state

    (incomplete, unused)
   */
   void setActions();

   PartymanConfigDialog    *mpConfig; /*!< \brief \todo TODO */
   PartymanMainWindow      *mpPlaylist; /*!< \brief \todo TODO */
   Satellite               *mpSatellite; /*!< \brief \todo TODO */
   GenericSatelliteHandler *mpGenericSatelliteHandler; /*!< \brief \todo TODO */
   PlayerWidget            *mpPlayer[2]; /*!< \brief \todo TODO */
   QIcon                   mPartymanIcon; /*!< \brief \todo TODO */
   QIcon                   mStopIcon; /*!< \brief \todo TODO */
   QIcon                   mPlayIcon; /*!< \brief \todo TODO */
   QIcon                   mPauseIcon; /*!< \brief \todo TODO */
   QIcon                   mSkipIcon; /*!< \brief \todo TODO */
   QIcon                   mLoadIcon; /*!< \brief \todo TODO */
   QPushButton             *mpSettingsButton; /*!< \brief \todo TODO */
   QToolButton             *mpStartButton; /*!< \brief \todo TODO */
   QToolButton             *mpSkipButton; /*!< \brief \todo TODO */
   QSystemTrayIcon         *mpTrayIcon; /*!< \brief \todo TODO */
   QMenu                   *mpTrayIconStopMenu; /*!< \brief \todo TODO */
   QMenu                   *mpTrayIconPlayMenu; /*!< \brief \todo TODO */
   QMenu                   *mpStartButtonMenu; /*!< \brief \todo TODO */
   QAction                 *mpPlayAction; /*!< \brief \todo TODO */
   QAction                 *mpSkipAction; /*!< \brief \todo TODO */
   QAction                 *mpPauseAction; /*!< \brief \todo TODO */
   QAction                 *mpStopAction; /*!< \brief \todo TODO */
   QAction                 *mpLoadAction; /*!< \brief \todo TODO */
   QTimer                  *mpTrayIconClickTimer; /*!< \brief \todo TODO */
   bool                    mConnected; /*!< \brief \todo TODO */
   bool                    mPaused; /*!< \brief \todo TODO */
   bool                    mKioskMode; /*!< \brief \todo TODO */
   QProcess                *mpDerMixDprocess; /*!< \brief \todo TODO */
   QProcess                *mpLoggerProcess; /*!< \brief \todo TODO */
   bool                    mWaitForDerMixD; /*!< \brief \todo TODO */
   bool                    mDerMixDstarted; /*!< \brief \todo TODO */
   QByteArray              mLastP0p; /*!< \brief \todo TODO */
};

/*! @} */

#endif
