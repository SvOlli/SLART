/**
 * src/apps/Stripped/CDDB.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDDB_HPP
#define CDDB_HPP CDDB_HPP

#include <QWidget>

#include <QMutex>

class QComboBox;
class QFile;
class QHttp;
class QLabel;

class CDToc;


class CDDB : public QWidget
{
Q_OBJECT
   
public:
   CDDB( CDToc *toc, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
   /* query freedb for possible matches */
   void query( const QString &querystring );
   
public slots:
   /* callback for QHttp */
   void httpRequestFinished( int requestId, bool error );
   /* request http read for the cue sheet from freedb */
   void read( const QString &querystring );
   /* cancel the request */
   void cancel();
   
signals:
   /* toc has been updated */
   void tocUpdated();
   
private:
   CDDB( const CDDB &other );
   CDDB &operator=( const CDDB &other );
   
   /* initiate the read request for freedb */
   void genericrequest( const QString &cmd );
   
   bool         mRequestIsQuery;
   bool         mCancel;
   CDToc        *mpToc;
   QLabel       *mpCount;
   QComboBox    *mpHits;
   int          mHttpGetId;
   QHttp        *mpHttp;
   QFile        *mpFile;
   QMutex       mMutex;
};

#endif
