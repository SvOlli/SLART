/**
 * CDDB.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CDDB_HPP
#define CDDB_HPP CDDB_HPP

#include <QWidget>
#include <QMutex>

class QLabel;
class QComboBox;
class QHttp;
class QFile;

class CDToc;

class CDDB : public QWidget
{
Q_OBJECT

public:
   CDDB( CDToc *toc, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   void query( const QString &querystring );

public slots:
   void httpRequestFinished( int requestId, bool error );
   void read( const QString &querystring );
   void cancel();

signals:
   void tocUpdated();

private:
   CDDB( const CDDB &other );
   CDDB &operator=( const CDDB &other );

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
