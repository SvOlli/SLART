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
#include <QTcpSocket>

class QTcpSocket;
class QLineEdit;
class QLabel;
class QScrollArea;
class QSlider;
class ControlWidget;
class ScrollLine;

class PlayerWidget : public QWidget
{
Q_OBJECT

public:
   enum eState { disconnected, unloaded, loading, loaded, playing, ending, 
                 stopped, seeking, skipping, paused, endingpaused };
   
   PlayerWidget( int index, ControlWidget *controlWidget, Qt::WindowFlags flags = 0 );
   /* set the track info like filename */
   void setInfo( const QString &line );
   /* updating the time played */
   void setPlayingStatus( const QString &line );
   /* set playing state */
   void setState( eState state, const QString &parameter = QString() );
   /* connect to DerMixD */
   void connectTo( const QString &hostname, int port );
   /* disconnect from DerMixD */
   void disconnect();
   /* skip current track */
   void skip();
   /* get filename of current Track */
   QString getFileName();
   /* report playing state */
   eState getState() { return mPlayState; };
   /* toggle pause */
   void pause();
   /* (re-)read configuration */
   void readConfig();

public slots:
   /* lock seek slider from getting status updates */
   void lock();
   /* jump to a position in track */
   void seek();
   /* handle unload request */
   void unload( const QPoint &/*pos*/ );
   /* handle successful connection to DerMixD */
   void handleConnect();
   /* handle disconnect from DerMixD */
   void handleDisconnect();
   /* handle responses from DerMixD */
   void handleResponse();
   /* handle DerMixD connection errors */
   void handleError( QAbstractSocket::SocketError socketError );

private:
   enum eDerMixD { inNormal, inFullstat, inScan };
   
   PlayerWidget( const PlayerWidget &other );
   PlayerWidget &operator=( const PlayerWidget &other );

   /* convert seconds to a 0:00 timecode */
   static QString sec2minsec( const QString &seconds );
   /* send a command to DerMixD */
   void sendCommand( const QString &command, const QString &parameter = QString() );
   /* enable/disable watching of track status */
   void watch( bool turnWatchOn );
   /* adjust volume for normalization */
   void handleScan( const QString &data );

   int           mPlayer;
   bool          mAutoStart;
   bool          mShortPlay;
   bool          mConsole;
   long          mTotalTime;
   long          mFrequency;
   long          mSamples;
   long          mHeadStart;
   enum eState   mPlayState;
   enum eDerMixD mDerMixD;
   bool          mUpdateSlider;
   bool          mWatching;
   int           mNormalizeMode;
   double        mNormalizeValue;
   ControlWidget *mpControlWidget;
   ScrollLine    *mpScrollLine;
   QLabel        *mpStatusDisplay;
   QLabel        *mpTimeDisplay;
   QSlider       *mpPlayPosition;
   QTcpSocket    *mpSocket;
};

#endif
