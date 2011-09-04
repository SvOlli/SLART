/**
 * src/apps/Partyman/SearchWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
class GenericSatMsgHandler;
class Satellite;


class CommandLineHandler : public QObject
{
   Q_OBJECT

public:
   CommandLineHandler( const QStringList &list, QObject *parent = 0 );
   virtual ~CommandLineHandler();

public slots:
   /* another instance of Partyman was found on the bus */
   void gotPing();
   /* no Partyman was found, so queue it */
   void timeout();

signals:
   void done();

private:
   Satellite            *mpSatellite;
   GenericSatMsgHandler *mpGenericSatMsgHandler;
   bool                 mConnected;
   QStringList          mList;
};

#endif
