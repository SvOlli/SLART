/*
 * src/libs/Common/GenericSatelliteHandler.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GENERICSATELLITEHANDLER_HPP
#define GENERICSATELLITEHANDLER_HPP GENERICSATELLITEHANDLER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QByteArray;

/* forward declaration of local classes */
class Satellite;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/GenericSatelliteHandler_connect.dot" "Connect Graph"
*/
class GenericSatelliteHandler : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief howto startup this class

   */
   enum StartupMode { WithoutPing, WithPing, WithPingAndDialog };

   /*!
    \brief constructor

    \param satellite
    \param mode
    \param parent
   */
   GenericSatelliteHandler( Satellite *satellite, StartupMode mode,
                            QObject *parent = 0);

   /*!
    \brief destructor

   */
   virtual ~GenericSatelliteHandler();

   /*!
    \brief set message to send upon connection

    \param msg
   */
   void setConnectMsg( const QByteArray &msg = QByteArray() );

   /*!
    \brief create a Satellite instance according to Settings

    \param force force creation, even if disabled via settings, used for tests
    \return Satellite created Satellite instance
   */
   static void createSatellite( bool force = false );

   /*!
    \brief send a message via Satellite according to settings

    \param message message to send
    \return object that holds the message and emits "done()" when done/deleted
   */
   static QObject *send( const QByteArray &message );

public slots:
   /*!
    \brief handle the message

   */
   void handle( const QByteArray &msg );

signals:
   /*!
    \brief send out a reply

   */
   void reply( const QByteArray &msg );
   /*!
    \brief signalize to update config

   */
   void updateConfig();
   /*!
    \brief singalize that another instance is already running

   */
   void anotherInstance();

private slots:
   /*!
    \brief send the PNG command to see if another instance is running

   */
   void sendPing();

   /*!
    \brief handle connection (send out message stored via setConnectMsg())

   */
   void connected();

private:
   Q_DISABLE_COPY( GenericSatelliteHandler )

   /* for handling two instances running:
      show the error message and quit the program */
   void anotherInstanceMessage();

   QByteArray  mConnectMsg;
   Satellite   *mpSatellite;
   bool        mWithQuitDialog;
};

/*! @} */

#endif
