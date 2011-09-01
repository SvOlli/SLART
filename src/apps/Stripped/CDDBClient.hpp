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
#include <QList>
#include <QString>

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
 \brief handle freedb connection via cddb protocol over http

 \class CDDBClient CDDBClient.hpp "apps/Stripped/CDDBClient.hpp"
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
    \brief the type of the current FreeDB request

    \enum RequestType
   */
   enum RequestType { NoRequest, RequestQuery, RequestTracks };

   CDDBClient( CDInfo *cdinfo, QWidget *parent = 0 );
   virtual ~CDDBClient();

   /*!
    \brief getter for the replyHander property

    \fn replyHandler
   */
   RequestType requestType() const;
   /*!
    \brief setter for the replyHander property

    \fn setReplyHandler
    \param value
   */
   void setRequestType( RequestType value );

   /*!
    \brief getter for the status message

    \fn statusMessage
   */
   QString statusMessage() const;
   /*!
    \brief setter for the status message

    \fn setStatusMessage
    \param value
   */
   void setStatusMessage( const QString &value );

public slots:
   /*!
    \brief clear all data

    \fn clear
   */
   void clear();

   /*!
    \brief handle a change of combo box: load cd data from net or cd-text

    \fn handleComboBox
    \param index
   */
   void handleComboBox();

   /*!
    \brief split the artist and title

    \fn handleSplit
    \param token the text to split at
   */
   void handleSplit( const QString &token = QString() );

   /*!
    \brief handle the reply from the freedb server

    \fn handleServerReply
    \param reply
   */
   void handleServerReply( QNetworkReply *reply );

   /*!
    \brief called when entering mpStateEjected (no disc in drive)

    \fn handleStateEjected
   */
   void handleStateEjected();

   /*!
    \brief called when entering mpStateCleared (disc just got inserted, clear all)

    \fn handleStateCleared
   */
   void handleStateCleared();

   /*!
    \brief called when entering mpStateQuery (query the freedb for possible discs)

    \fn handleStateQuery
   */
   void handleStateQuery();

   /*!
    \brief called when entering mpStateRead (read track listing from freedb)

    \fn handleStateRead
   */
   void handleStateRead();

   /*!
    \brief called when entering mpStateCDText (read track listing from cdtext)

    \fn handleStateCDText
   */
   void handleStateCDText();

   /*!
    \brief called when entering mpStateDone (done with actions, waiting for user)

    \fn handleStateDone
   */
   void handleStateDone();

signals:
   /*!
    \brief used for state machine transitions: cd got inserted

    \fn cdinsert
   */
   void cdinsert();
   /*!
    \brief used for state machine transitions: query freedb for possible cds

    \fn query
   */
   void query();
   /*!
    \brief used for state machine transitions: read track info from cd-text

    \fn cdtext
   */
   void cdtext();
   /*!
    \brief used for state machine transitions: request a selected cd from freedb

    \fn select
   */
   void select();
   /*!
    \brief used for state machine transitions: eject cd

    \fn eject
   */
   void eject();
   /*!
    \brief used for state machine transitions: do automatic action (when enabled)

    \fn automatic
   */
   void automatic();
   /*!
    \brief used for state machine transitions: cancel current action

    \fn cancel
   */
   void cancel();
   /*!
    \brief used for state machine transitions: found cd(s) in query

    \fn found
   */
   void found();
   /*!
    \brief used for state machine transitions: got track data for a selected cd

    \fn gotdata
   */
   void gotdata();
   /*!
    \brief used for state machine transitions: an error occured

    \fn error
   */
   void error();

   /*!
    \brief signalize "net access"-state

    \fn stateNet
   */
   void stateNet();
   /*!
    \brief signalize "disc read"-state

    \fn stateDisc
   */
   void stateDisc();
   /*!
    \brief signalize that track info has been updated

    \fn infoUpdated
   */
   void infoUpdated();
   /*!
    \brief signalize the request to read cd-text

    \fn requestCDText
   */
   void requestCDText();
   /*!
    \brief send out info message

    \fn message
    \param message
   */
   void message( const QString &message = QString() );

private:
   CDDBClient( const CDDBClient &that );
   CDDBClient &operator=( const CDDBClient &that );

   /*!
    \brief send out a command to freedb server

    \fn startRequest
    \param cmd
    \param parameters
   */
   void startRequest( RequestType type, const QStringList &parameters );

   /*!
    \brief handle the "match a cd"-reply

    \fn handleQueryData
    \param reply TODO
   */
   void handleQueryData( QNetworkReply *reply );

   /*!
    \brief handle the "get cd data"-reply

    \fn handleQueryData
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
};

#endif
