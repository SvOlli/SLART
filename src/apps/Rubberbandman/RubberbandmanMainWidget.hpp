/*
 * src/apps/Rubberbandman/RubberbandmanMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef RUBBERBANDMANMAINWIDGET_HPP
#define RUBBERBANDMANMAINWIDGET_HPP RUBBERBANDMANMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QPixmap>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QPushButton;
class QTabWidget;

/* forward declaration of local classes */
class BrowseWidget;
class RubberbandmanConfigDialog;
class Database;
class DatabaseWidget;
class SatelliteWidget;


/*!
  \addtogroup Rubberbandman

  \brief application: tag editor and database manager

  Named after [the song by Yello](https://de.wikipedia.org/wiki/Yello/Diskografie#Singles).

  @{
  */

/*!
 \brief the main widget

 \dotfile "graphs/apps/Rubberbandman/RubberbandmanMainWidget_connect.dot" "Connect Graph"
*/
class RubberbandmanMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent \todo REPLACE STANDARD
    \param flags \todo REPLACE STANDARD
   */
   RubberbandmanMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );
   /*!
    \brief destructor

   */
   virtual ~RubberbandmanMainWidget();

public slots:
   /*!
    \brief switch to filesystem tab

   */
   void goToFilesystem();
   /*!
    \brief save current tab for restart

    \param tabNr
   */
   void handleTabChange( int tabNr );

   /*!
    \brief show database activity

    \param on
   */
   void databaseActive( bool on );

private:
   Q_DISABLE_COPY( RubberbandmanMainWidget )

   BrowseWidget               *mpBrowseWidget; /*!< \brief \todo complete documentation */
   SatelliteWidget            *mpSatelliteWidget; /*!< \brief \todo complete documentation */
   DatabaseWidget             *mpDatabaseWidget; /*!< \brief \todo complete documentation */
   QTabWidget                 *mpTabs; /*!< \brief \todo complete documentation */
   QPushButton                *mpSettingsButton; /*!< \brief \todo complete documentation */
   QLabel                     *mpDatabaseActivity; /*!< \brief small widget indicating database activity */
   RubberbandmanConfigDialog  *mpConfigDialog; /*!< \brief \todo complete documentation */
   QPixmap                    mActiveLED; /*!< \brief \todo TODO */
   QPixmap                    mIdleLED; /*!< \brief \todo TODO */
};

/*! @} */

#endif
