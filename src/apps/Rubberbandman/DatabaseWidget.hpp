/**
 * src/apps/Rubberbandman/DatabaseWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DATABASEWIDGET_HPP
#define DATABASEWIDGET_HPP DATABASEWIDGET_HPP

#include <QWidget>

#include <QHostInfo>

#include "TrackInfo.hpp"

class QFileInfo;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlTableModel;
class QString;
class QTableView;

class Database;
class DatabaseWorker;


class DatabaseWidget : public QWidget
{
Q_OBJECT

public:
   DatabaseWidget( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* handle update button */
   void handleUpdate( bool checked );
   /* handle cleanup button */
   void handleCleanup( bool checked );
   /* handle import button */
   void handleImport( bool checked );
   /* pop up requester for selecting base dir */
   void setBaseDir();
   /* update buttons to show if base dir is valid */
   void checkValidDir( const QString &dirName );
   /* update output from worker */
   void handleProgress( int checked, int processed );
   /* handle finishing worker */
   void handleFinished();
   /* read the Partyman configuration to determine if running in local mode */
   void readPartymanConfig( const QHostInfo &hi = QHostInfo() );

signals:
   /* for Sorcerer: signal, that the database has been updated / created */
   void databaseUpdated();

private:
   DatabaseWidget( const DatabaseWidget &other );
   DatabaseWidget &operator=( const DatabaseWidget &other );

   void disableButtons( bool disable );

   Database       *mpDatabase;
   DatabaseWorker *mpDatabaseWorker;
   QLineEdit      *mpBaseDir;
   QPushButton    *mpUpdateButton;
   QPushButton    *mpCleanupButton;
   QPushButton    *mpImportButton;
   QLabel         *mpMessage;
   QLabel         *mpPartymanInfo;
#if 0
   QSqlTableModel *mpTableModel;
   QTableView     *mpTableView;
#endif
   bool           mPartymanLocal;
   QString        mCheckedText;
   QString        mProcessedText;
};

#endif
