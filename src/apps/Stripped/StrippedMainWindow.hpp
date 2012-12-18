/*
 * src/apps/Stripped/StrippedMainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef STRIPPEDMAINWINDOW_HPP
#define STRIPPEDMAINWINDOW_HPP STRIPPEDMAINWINDOW_HPP

/* base class */
#include <QMainWindow>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QIcon;
class QLabel;
class QPushButton;
class QString;

/* forward declaration of local classes */
class CDInfo;
class CDDBClient;
class CDEdit;
class CDReader;
class StrippedConfigDialog;
class GenericSatelliteHandler;
class ParanoiaStatus;
class Satellite;

/*!
  \addtogroup Stripped Stripped: CD-ripping

 @{
 */

/*!
 \brief the main window

 \dotfile "graphs/apps/Stripped/StrippedMainWindow_connect.dot" "Connect Graph"
*/
class StrippedMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
    \param flags
   */
   StrippedMainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~StrippedMainWindow();

public slots:
   /*!
    \brief \todo

    \param enabled
   */
   void changeEncoderIcon( QAction *action = 0 );

   /*!
    \brief handle eject button

   */
   void eject();

   /*!
    \brief set buttons active according to state "no drive found"

   */
   void stateNoDrive();

   /*!
    \brief set buttons active according to state "no disc inserted"

   */
   void stateNoDisc();

   /*!
    \brief set buttons active according to state "disc read"

   */
   void stateDisc();

   /*!
    \brief set buttons active according to state "scanning the disc"

   */
   void stateScan();

   /*!
    \brief set buttons active according to state "connecting to freedb"

   */
   void stateNet();

   /*!
    \brief set buttons active according to state "ripping tracks"

   */
   void stateRip();

   /*!
    \brief handle request for new icon and title

    \param icon
    \param title
   */
   void changeTitle( const QIcon &icon, const QString &title );

protected:
   /*!
    \brief intercept for writing the settings

    \param event
   */
   virtual void closeEvent( QCloseEvent *event );

   /*!
    \brief very ugly workaround for wrong position restoration on Ubuntu

    \param event
   */
   virtual bool event( QEvent *event );

private:
   Q_DISABLE_COPY( StrippedMainWindow )

   /*!
    \brief \todo

    \param bar
    \param action
   */
   void addToolAction( QToolBar *bar, QAction *action, bool checkbox = false);

   int         mForbidMove;

   Satellite               *mpSatellite;
   GenericSatelliteHandler *mpGenericSatelliteHandler;
   CDInfo                  *mpCDInfo;
   CDDBClient              *mpCDDBClient;
   CDEdit                  *mpCDEdit;
   CDReader                *mpCDReader;
   ParanoiaStatus          *mpParanoiaStatus;
   QLabel                  *mpMessage;
   StrippedConfigDialog    *mpConfigDialog;

   QAction     *mpActionSettings;
   QAction     *mpActionCancel;
   QAction     *mpActionScanCD;
   QAction     *mpActionRipTracks;
   QAction     *mpActionEject;
   QAction     *mpActionToggleRip;
   QAction     *mpActionToggleEnqueue;
   QAction     *mpActionCopyArtist;
   QAction     *mpActionNormArtist;
   QAction     *mpActionNormTitle;
   QAction     *mpActionCopyYear;
};

/*! @} */

#endif
