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

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QPushButton;
class QTabWidget;

/* forward declaration of local classes */
class BrowseWidget;
class ConfigDialog;
class Database;
class DatabaseWidget;
class SatelliteWidget;


/*!
  \addtogroup Rubberbandman Rubberbandman: tag editor and database manager
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
    \brief

    \param parent
    \param flags
   */
   RubberbandmanMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief

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

signals:
   /*!
    \brief

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( RubberbandmanMainWidget )

   BrowseWidget      *mpBrowseWidget; /*!< \todo */
   SatelliteWidget   *mpSatelliteWidget; /*!< \todo */
   DatabaseWidget    *mpDatabaseWidget; /*!< \todo */
   QTabWidget        *mpTabs; /*!< \todo */
   QPushButton       *mpSettingsButton; /*!< \todo */
   QLabel            *mpDatabaseActivity; /*!< \brief small widget indicating database activity */
   ConfigDialog      *mpConfigDialog; /*!< \todo */
};

/*! @} */

#endif
