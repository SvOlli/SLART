/**
 * src/apps/Stripped/CDDBClient.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDDBCLIENT_HPP
#define CDDBCLIENT_HPP CDDBCLIENT_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QLabel;
class QNetworkAccessManager;
class QNetworkReply;
class QState;
class QStateMachine;
class QTimer;

/* forward declaration of local classes */
class CDInfo;


/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief handle freedb connection via cddb protocol over http

*/
class CDDBClient : public QWidget
{
   Q_OBJECT
   Q_ENUMS(RequestType)

   Q_PROPERTY( RequestType requestType
               READ requestType
               WRITE setRequestType )
   Q_PROPERTY( QString statusMessage
               READ statusMessage
               WRITE setStatusMessage )

public:
   /*!
    \brief the type of the current CDDB request

    \enum RequestType
   */
   enum RequestType { NoRequest, RequestQuery, RequestTracks };

   CDDBClient( CDInfo *cdinfo, QWidget *parent = 0 );
   virtual ~CDDBClient();

   /*!
    \brief getter for the replyHander property

   */
   RequestType requestType() const;
   /*!
    \brief setter for the replyHander property

    \param value
   */
   void setRequestType( RequestType value );

   /*!
    \brief getter for the status message

   */
   QString statusMessage() const;
   /*!
    \brief setter for the status message

    \param value
   */
   void setStatusMessage( const QString &value );

public slots:
   /*!
    \brief clear all data

   */
   void clear();

   /*!
    \brief handle a change of combo box: load cd data from net or cd-text

    \param index
   */
   void handleComboBox();

   /*!
    \brief split the artist and title

    \param token the text to split at
   */
   void handleSplit( const QString &token = QString() );

   /*!
    \brief handle the reply from the freedb server

    \param reply
   */
   void handleServerReply( QNetworkReply *reply );

   /*!
    \brief called when entering mpStateEjected (no disc in drive)

   */
   void handleStateEjected();

   /*!
    \brief called when entering mpStateCleared (disc just got inserted, clear all)

   */
   void handleStateCleared();

   /*!
    \brief called when entering mpStateQuery (query the freedb for possible discs)

   */
   void handleStateQuery();

   /*!
    \brief called when entering mpStateRead (read track listing from freedb)

   */
   void handleStateRead();

   /*!
    \brief called when entering mpStateCDText (read track listing from cdtext)

   */
   void handleStateCDText();

   /*!
    \brief called when entering mpStateDone (done with actions, waiting for user)

   */
   void handleStateDone();

signals:
   /*!
    \brief used for state machine transitions: cd got inserted

   */
   void cdinsert();
   /*!
    \brief used for state machine transitions: query freedb for possible cds

   */
   void query();
   /*!
    \brief used for state machine transitions: read track info from cd-text

   */
   void cdtext();
   /*!
    \brief used for state machine transitions: request a selected cd from freedb

   */
   void select();
   /*!
    \brief used for state machine transitions: eject cd

   */
   void eject();
   /*!
    \brief used for state machine transitions: do automatic action (when enabled)

   */
   void automatic();
   /*!
    \brief used for state machine transitions: cancel current action

   */
   void cancel();
   /*!
    \brief used for state machine transitions: found cd(s) in query

   */
   void found();
   /*!
    \brief used for state machine transitions: got track data for a selected cd

   */
   void gotdata();

   /*!
    \brief signalize "net access"-state

   */
   void stateNet();

   /*!
    \brief signalize "disc read"-state

   */

   void stateDisc();
   /*!
    \brief signalize that track info has been updated

   */
   void requestCDText();

   /*!
    \brief send out info message

    \param message
   */
   void message( const QString &message = QString() );

private:
   CDDBClient( const CDDBClient &that );
   CDDBClient &operator=( const CDDBClient &that );

   /*!
    \brief send out a command to freedb server

    \param cmd type of request (query cd or read data)
    \param parameters parameters needed for cmd
   */
   void startRequest( RequestType type, const QStringList &parameters );

   /*!
    \brief handle the "match a cd"-reply

    \param reply TODO
   */
   void handleQueryData( QNetworkReply *reply );

   /*!
    \brief handle the "get cd data"-reply

    \param reply TODO
   */
   void handleReadData( QNetworkReply *reply );

   RequestType             mRequestType;
   QStateMachine           *mpStateMachine;
   QState                  *mpStateEjected;
   QState                  *mpStateCleared;
   QState                  *mpStateQuery;
   QState                  *mpStateRead;
   QState                  *mpStateCDText;
   QState                  *mpStateDone;
   QLabel                  *mpCount;
   QComboBox               *mpHits;
   QLabel                  *mpSplitLabel;
   QComboBox               *mpSplit;
   QNetworkAccessManager   *mpNAM;
   CDInfo                  *mpCDInfo;
   QTimer                  *mpMessageResetTimer;
   QString                 mReadCDTextMessage;
   QString                 mQueryCDDBMessage;
   QString                 mStatusMessage;
   QStringList             mLastParameter;
};

/*! @} */

#endif
