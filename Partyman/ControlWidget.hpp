/**
 * ControlWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONTROLWIDGET_HPP
#define CONTROLWIDGET_HPP CONTROLWIDGET_HPP

#include "PlayerWidget.hpp"
#include "SLATCom.hpp"

#include <QWidget>
#include <QIcon>
#include <QUdpSocket>
#include <QProcess>
class PlayerWidget;
class ConfigDialog;
class QPushButton;
class QMenu;
class QAction;


class ControlWidget : public QWidget
{
Q_OBJECT
public:
   enum eErrorCode { noError, noConnection, connectionLost, wrongVersion };
   
   ControlWidget( ConfigDialog *config, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~ControlWidget();
   /* callback for player to request next track */
   void loadNext( int player );
   /* set the filename to load */
   void setFileName( int player, const QString &fileName );
   /* callback for player to report its state */
   void reportState( int player, PlayerWidget::eState state );
   /* add files to playlist (for unloading) */
   void addToPlaylist( const QStringList &entries );
   /* run SLATCommunication & execute external logger */
   void log( const QString &udpEvent, const QString &logEvent, const QString &data = QString() );

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
   void nextTrack();
   /* handle (un)pausing */
   void handlePause( bool reset = false );
   /* handle remote requests */
   void handleSLAT( const QStringList &package );
   /* handle process startup */
   void handleDerMixDstartup();
   /* handle process finish */
   void handleDerMixDfinish( int exitCode, QProcess::ExitStatus exitStatus );
   /* handle process error */
   void handleDerMixDerror( QProcess::ProcessError error );

signals:
   /* request next filename */
   void requestNextTrack( int player );
   /* request for adding file to playlist (used for unload) */
   void requestAddToPlaylist( const QStringList &entries, bool atStart );
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );
   /* signal if a connection has been established or cut off */
   void signalConnected( bool connected );

private:
   ControlWidget( const ControlWidget &other );
   ControlWidget &operator=( const ControlWidget &other );

   ConfigDialog   *mpConfig;
   int            mLastLoad;
   QLabel         *mpLogo;
   QPushButton    *mpConnectButton;
   QPushButton    *mpSkipButton;
   PlayerWidget   *mpPlayer[2];
   bool           mConnected;
   bool           mPaused;
   QIcon          mStopIcon;
   QIcon          mPlayIcon;
   QIcon          mPauseIcon;
   QMenu          *mpDisconnectMenu;
   QAction        *mpPauseAction;
   QAction        *mpDisconnectAction;
   SLATCom        mSLATCom;
   QProcess       mDerMixDprocess;
   QProcess       mLoggerProcess;
   bool           mWaitForDerMixD;
   bool           mDerMixDstarted;
};

#endif
