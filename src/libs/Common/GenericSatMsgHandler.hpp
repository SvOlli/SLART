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
   /*  */
   void handle( const QByteArray &msg );

signals:
   /*  */
   void reply( const QByteArray &msg );
   /*  */
   void updateConfig();

private:
   GenericSatMsgHandler( const GenericSatMsgHandler &other );
   GenericSatMsgHandler &operator=( const GenericSatMsgHandler &other );

   Satellite   *mpSatellite;
};

#endif
