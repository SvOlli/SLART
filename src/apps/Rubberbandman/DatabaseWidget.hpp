/*
 * src/apps/Rubberbandman/DatabaseWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DATABASEWIDGET_HPP
#define DATABASEWIDGET_HPP DATABASEWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QHostInfo>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QFileInfo;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlTableModel;
class QString;
class QTableView;

/* forward declaration of local classes */
class Database;
class DatabaseWorker;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Rubberbandman/DatabaseWidget_connect.dot" "Connect Graph"
*/
class DatabaseWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   DatabaseWidget( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~DatabaseWidget();

public slots:
   /*!
    \brief handle update button

   */
   void handleUpdate( bool checked );
   /*!
    \brief handle cleanup button

   */
   void handleCleanup( bool checked );
   /*!
    \brief handle import button

   */
   void handleImport( bool checked );
   /*!
    \brief pop up requester for selecting base dir

   */
   void setBaseDir();
   /*!
    \brief update buttons to show if base dir is valid

   */
   void checkValidDir( const QString &dirName );
   /*!
    \brief update output from worker

   */
   void handleProgress( int checked, int processed );
   /*!
    \brief handle finishing worker

   */
   void handleFinished();
   /*!
    \brief read the Partyman configuration to determine if running in local mode

   */
   void readPartymanConfig( const QHostInfo &hi = QHostInfo() );

signals:
   /*!
    \brief for Sorcerer: signal, that the database has been updated / created

   */
   void databaseUpdated();

private:
   Q_DISABLE_COPY( DatabaseWidget )

   /*!
    \brief disable all buttons

    \param disable
   */
   void disableButtons( bool disable );

   DatabaseWorker *mpDatabaseWorker;
   QLineEdit      *mpBaseDir;
   QPushButton    *mpUpdateButton;
   QPushButton    *mpCleanupButton;
   QPushButton    *mpImportButton;
   QLabel         *mpMessage;
   QLabel         *mpPartymanInfo;
   bool           mPartymanLocal;
   QString        mCheckedText;
   QString        mProcessedText;
};

/*! @} */

#endif
