/**
 * src/apps/Rubberbandman/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QTabWidget;
class QPushButton;

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
 \brief

*/
class MainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief

    \param parent
    \param flags
   */
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief

   */
   virtual ~MainWidget();

public slots:
   /* switch to filesystem tab */
   /*!
    \brief

   */
   void goToFilesystem();
   /* save current tab for restart */
   /*!
    \brief

    \param tabNr
   */
   void handleTabChange( int tabNr );

signals:
   /*!
    \brief

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( MainWidget )

   BrowseWidget      *mpBrowseWidget; /*!< TODO */
   SatelliteWidget   *mpSatelliteWidget; /*!< TODO */
   DatabaseWidget    *mpDatabaseWidget; /*!< TODO */
   QTabWidget        *mpTabs; /*!< TODO */
   QPushButton       *mpSettingsButton; /*!< TODO */
   ConfigDialog      *mpConfigDialog; /*!< TODO */
};

/*! @} */

#endif
