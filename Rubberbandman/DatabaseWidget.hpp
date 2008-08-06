/**
 * DatabaseWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DATABASEWIDGET_HPP
#define DATABASEWIDGET_HPP DATABASEWIDGET_HPP

#include <QWidget>
#include "TrackInfo.hpp"

class QPushButton;
class QLabel;
class QString;
class QSqlTableModel;
class QFileInfo;
class QTableView;
class Database;
class DatabaseWorker;
class QLineEdit;

class DatabaseWidget : public QWidget
{
Q_OBJECT

public:
   DatabaseWidget( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /*  */
   void handleUpdate( bool checked );
   /*  */
   void handleCleanup( bool checked );
   /*  */
   void handleImport( bool checked );
   /*  */
   void setBaseDir();
   /*  */
   void checkValidDir( const QString &dirName );
   /*  */
   void handleProgress( int checked, int processed );
   /*  */
   void handleFinished();

signals:
   void databaseOk();

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
#if 0
   QSqlTableModel *mpTableModel;
   QTableView     *mpTableView;
#endif
   QString        mCheckedText;
   QString        mProcessedText;
};

#endif
