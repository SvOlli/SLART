/**
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


class GenericSatMsgHandler : public QObject
{
Q_OBJECT

public:
   GenericSatMsgHandler( Satellite *satellite );
   virtual ~GenericSatMsgHandler();

public slots:
   /* send the PNG command to see if another instance is running */
   void sendPing( bool withQuitDialog = false );
   /* handle the message */
   void handle( const QByteArray &msg );

signals:
   /* send out a reply */
   void reply( const QByteArray &msg );
   /* signalize to update config */
   void updateConfig();
   /* singalize that another instance is already running */
   void anotherInstance();

private:
   GenericSatMsgHandler( const GenericSatMsgHandler &other );
   GenericSatMsgHandler &operator=( const GenericSatMsgHandler &other );

   /* for handling two instances running:
      show the error message and quit the program */
   void anotherInstanceMessage();

   Satellite   *mpSatellite;
   bool        mWithQuitDialog;
};

#endif
