/**
 * src/apps/Rubberbandman/SLARTComWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
class GenericSatMsgHandler;
class InfoEdit;
class Satellite;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class SatelliteWidget : public QWidget
{
   Q_OBJECT

public:
   SatelliteWidget( QWidget *parent = 0 );

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
   SatelliteWidget( const SatelliteWidget &that );
   SatelliteWidget &operator=( const SatelliteWidget &that );

   DatabaseInterface    *mpDatabase;
   InfoEdit             *mpInfoEdit;
   Satellite            *mpSatellite;
   GenericSatMsgHandler *mpGenericSatMsgHandler;
   QPushButton          *mpNowPlaying;
   QPushButton          *mpShowInFilesystem;
   QPushButton          *mpGetRandom;
   TrackInfo            mTrackInfo;
};

/*! @} */

#endif
