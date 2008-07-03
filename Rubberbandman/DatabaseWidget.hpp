/**
 * DatabaseWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DATABASEWIDGET_HPP
#define DATABASEWIDGET_HPP DATABASEWIDGET_HPP

#include <QWidget>
//#include <QDir>
//#include <QTableView>
#include "TrackInfo.hpp"

class QPushButton;
class QString;
class QSqlTableModel;
class QFileInfo;
class QTableView;
class Database;

class DatabaseWidget : public QWidget
{
Q_OBJECT

public:
   DatabaseWidget( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   bool updateTrackInfoFromFile( TrackInfo *trackInfo, const QString &fileName );

public slots:
   void handleUpdate();
   void handleCleanup();
   void handleFile( const QFileInfo &fileInfo );

private:
   DatabaseWidget( const DatabaseWidget &other );
   DatabaseWidget &operator=( const DatabaseWidget &other );

   Database       *mpDatabase;
#if 0
   QSqlTableModel *mpTableModel;
   QTableView     *mpTableView;
#endif
};

#endif
