/**
 * src/tests/TestAppWebServer/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QTime>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QModelIndex;
class QSqlQueryModel;
class QSqlTableModel;
class QTableView;
class QTableWidget;

/* forward declaration of local classes */
class Database;
class EvilTableWidget;


class MainWidget : public QWidget
{
   Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();

public slots:
   void fillTableFull();
   void handleClicked( const QModelIndex &index );
   void changeHeight( int fromRow, int toRow );

signals:

private:
   MainWidget( const MainWidget &that );
   MainWidget &operator=( const MainWidget &that );

   Database             *mpDatabase;
   QSqlQueryModel       *mpModel;
   QTableView           *mpView;
   QLabel               *mpInfo;
   QTime                mTime;
};

#endif
