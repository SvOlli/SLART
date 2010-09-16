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

/* forward declaration of local classes */
class CDInfo;


class CDDBClient : public QWidget
{
Q_OBJECT

public:
   CDDBClient( CDInfo *cdinfo, QWidget *parent = 0 );
   virtual ~CDDBClient();

   /* remove all previous data */
   void clear();

public slots:
   /* cancel the request */
   void cancel();
   /* handle a change of combo box: load cd data from net or cd-text */
   void handleComboBox( int index = 1 );
   /* split the artist and title */
   void handleSplit( const QString &token = QString() );
   /* handle the "match a cd"-request data */
   void handleQueryData( QNetworkReply *reply );
   /* handle the "get cd data"-request data */
   void handleReadData( QNetworkReply *reply );

signals:
   /* signalize "net access"-state */
   void stateNet();
   /* signalize "disc read"-state */
   void stateDisc();
   /* signalize that track info has been updated */
   void infoUpdated();
   /* signalize the request to read cd-text */
   void requestCDText();
   /* send out info message */
   void message( const QString &message = QString() );

private:
   CDDBClient( const CDDBClient &other );
   CDDBClient &operator=( const CDDBClient &other );

   /* send out a command to freedb server */
   void startRequest( const QString &cmd, const QStringList &parameters );

   QLabel                  *mpCount;
   QComboBox               *mpHits;
   QLabel                  *mpSplitLabel;
   QComboBox               *mpSplit;
   QNetworkAccessManager   *mpNAM;
   CDInfo                  *mpCDInfo;
   QList<QNetworkReply*>   mQueryReplies;
   QList<QNetworkReply*>   mReadReplies;
   QString                 mReadCDTextMessage;
   QString                 mQueryCDDBMessage;
};

#endif
