/*
 * src/apps/Innuendo/InnuendoMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef INNUENDOMAINWIDGET_HPP
#define INNUENDOMAINWIDGET_HPP INNUENDOMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */
/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QKeyEvent;
class QLabel;
class QListWidgetItem;
class QIcon;
class QPushButton;
class QSpinBox;
class QStringList;

/* forward declaration of local classes */
class InnuendoConfigDialog;
class DropDialog;
class ExecButton;
class GenericSatelliteHandler;
class LogListWidget;
class Satellite;


/*!
  \addtogroup Innuendo

  \brief application: central application

  Named after [the song by Queen](https://en.wikipedia.org/wiki/Innuendo_%28song%29).

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Innuendo/InnuendoMainWidget_connect.dot" "Connect Graph"
*/
class InnuendoMainWidget : public QWidget
{
   Q_OBJECT

public:
   InnuendoMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~InnuendoMainWidget();

public slots:
   /*!
    \brief re-read config

   */
   void readConfig();
   /*!
    \brief handle the ping button

   */
   void handlePingButton();
   /*!
    \brief handle SLART message

   */
   void handleSatellite( const QByteArray &msg );
   /*!
    \brief copy clicked line of message buffer to clipboard

   */
   void listWidgetItemToClipboard( QListWidgetItem *item );

private slots:
   /*!
    \brief autostart

   */
   void autostart();
   /*!
    \brief prohibit autostart, in case Innuendo is already running

   */
   void noAutostart();

protected:
   /*!
    \brief for implementing dropping

   */
   virtual void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief for implementing dropping

   */
   virtual void dropEvent( QDropEvent *event );

private:
   Q_DISABLE_COPY( InnuendoMainWidget )

   Satellite               *mpSatellite;
   GenericSatelliteHandler *mpGenericSatelliteHandler;
   LogListWidget           *mpMessageBuffer;
   QPushButton             *mpSettingsButton;
   QPushButton             *mpPingButton;
   InnuendoConfigDialog    *mpConfig;
   DropDialog              *mpDropDialog;
   ExecButton              **mpExecButtons;
   int                     mNumExecButtons;
   QStringList             mAutostart;
};

/*! @} */

#endif
