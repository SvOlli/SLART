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

   /*  */
   void clear();

public slots:
   /*  */
   void cancel();
   /*  */
   void handleComboBox( int index = 1 );
   /*  */
   void handleSplit( const QString &token = QString() );
   /*  */
   void handleQueryData( QNetworkReply *reply );
   /*  */
   void handleReadData( QNetworkReply *reply );

signals:
   /*  */
   void infoUpdated();
   /*  */
   void requestCDText();
   /*  */
   void message( const QString &message = QString() );

private:
   CDDBClient( const CDDBClient &other );
   CDDBClient &operator=( const CDDBClient &other );

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
