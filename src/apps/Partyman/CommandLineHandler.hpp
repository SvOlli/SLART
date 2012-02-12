/*
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


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

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
   Satellite            *mpSatellite; /*!< \brief satellite instance */
   GenericSatMsgHandler *mpGenericSatMsgHandler; /*!< \brief generic satellite message handler instance */
   bool                 mConnected; /*!< \brief connected to bus? */
   QStringList          mList; /*!< \brief list to send */
};

/*! @} */

#endif
