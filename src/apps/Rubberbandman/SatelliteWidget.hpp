/*
 * src/apps/Rubberbandman/SLARTComWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SLARTCOMWIDGET_HPP
#define SLARTCOMWIDGET_HPP SLARTCOMWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QPushButton;
class QString;

/* forward declaration of local classes */
class DatabaseInterface;
class GenericSatelliteHandler;
class InfoEdit;
class Satellite;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Rubberbandman/SatelliteWidget_connect.dot" "Connect Graph"
*/
class SatelliteWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   SatelliteWidget( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~SatelliteWidget();

public slots:
   /*!
    \brief handle SLART message to read tags of current track

   */
   void handleSatellite( const QByteArray &message );
   /*!
    \brief handle the now playing button

   */
   void handleNowPlaying();
   /*!
    \brief handle the show in filesystem button

   */
   void handleShowInFilesystem();
   /*!
    \brief handle the "Get Random Track" button

   */
   void handleGetRandom();
   /*!
    \brief handle the "Enqueue In Partyman" button

   */
   void handleEnqueue();

signals:
   /*!
    \brief emit track path from show in filesystem button

   */
   void showInFilesystem( const QString &path );
   /*!
    \brief emit that partyman did an update on the config

   */
   void partymanConfigUpdate();

private:
   Q_DISABLE_COPY( SatelliteWidget )

   DatabaseInterface       *mpDatabase;
   InfoEdit                *mpInfoEdit;
   Satellite               *mpSatellite;
   GenericSatelliteHandler *mpGenericSatelliteHandler;
   QPushButton             *mpNowPlaying;
   QPushButton             *mpShowInFilesystem;
   QPushButton             *mpGetRandom;
   QPushButton             *mpEnqueue;
   TrackInfo               mTrackInfo;
};

/*! @} */

#endif
