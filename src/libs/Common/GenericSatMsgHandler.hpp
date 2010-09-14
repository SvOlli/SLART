/**
 * src/libs/Common/GenericSatMsgHandler.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef GENERICSATMSGHANDLER_HPP
#define GENERICSATMSGHANDLER_HPP

#include <QObject>

class QByteArray;

class Satellite;


class GenericSatMsgHandler : public QObject
{
Q_OBJECT

public:
   GenericSatMsgHandler( Satellite *satellite );
   virtual ~GenericSatMsgHandler();

public slots:
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

   Satellite   *mpSatellite;
};

#endif
