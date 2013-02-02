/*
 * src/apps/Partyman/SearchTrackWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef COMMANDLINEHANDLER_HPP
#define COMMANDLINEHANDLER_HPP COMMANDLINEHANDLER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class GenericSatelliteHandler;
class Satellite;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief helper class to enqueue tracks from command line

 Creating an instance of this class will try to connect to an already running
 Partyman. If it succeeds, it will add the tracks provided as parameter, if it
 fails, it will add them to the playlist stored in the settings. On completing
 either task it'll delete itself and quit the application.

 \dotfile "graphs/apps/Partyman/CommandLineHandler_connect.dot" "Connect Graph"
*/
class CommandLineHandler : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param list list to send
    \param parent parent object
   */
   CommandLineHandler( const QStringList &list, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~CommandLineHandler();

public slots:
   /*!
    \brief another instance of Partyman was found on the bus

   */
   void gotPing();
   /*!
    \brief no Partyman was found, so queue it

   */
   void timeout();

signals:
   /*!
    \brief signalize completion of task

   */
   void done();

private:
   Satellite               *mpSatellite; /*!< \brief satellite instance */
   GenericSatelliteHandler *mpGenericSatelliteHandler; /*!< \brief generic satellite message handler instance */
   bool                    mConnected; /*!< \brief connected to bus? */
   QStringList             mList; /*!< \brief list to send */
};

/*! @} */

#endif
