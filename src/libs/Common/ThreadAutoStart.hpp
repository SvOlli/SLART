/*
 * src/libs/Common/ThreadAutoStart.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef THREADAUTOSTART_HPP
#define THREADAUTOSTART_HPP THREADAUTOSTART_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
class QThread;

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief this is a small helper class to start threads immediately after entering
 the main loop

 This is necessary, because
 \code QTimer::singleShot( 0, thread, SLOT(start()) ); \endcode
 will cause the event to be enqueued in the eventloop of the thread, causing
 a chicken/egg-like problem.
*/
/*
 \dotfile "graphs/libs/Common/ThreadAutoStart_connect.dot" "Connect Graph"
*/
class ThreadAutoStart : public QObject
{
   Q_OBJECT

public:
   ThreadAutoStart( QThread *thread );
   virtual ~ThreadAutoStart();

private slots:
   void start();

private:
   Q_DISABLE_COPY( ThreadAutoStart )

   QThread     *mpThread;
};

/*! @} */

#endif
