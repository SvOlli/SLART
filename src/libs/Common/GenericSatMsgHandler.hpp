/*
 * src/libs/Common/GenericSatMsgHandler.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GENERICSATMSGHANDLER_HPP
#define GENERICSATMSGHANDLER_HPP GENERICSATMSGHANDLER_HPP

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
 \brief TODO

*/
class GenericSatMsgHandler : public QObject
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
   */
   GenericSatMsgHandler( Satellite *satellite, StartupMode mode );

   /*!
    \brief destructor

   */
   virtual ~GenericSatMsgHandler();

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

private:
   Q_DISABLE_COPY( GenericSatMsgHandler )

   /* for handling two instances running:
      show the error message and quit the program */
   void anotherInstanceMessage();

   Satellite   *mpSatellite;
   bool        mWithQuitDialog;
};

/*! @} */

#endif
