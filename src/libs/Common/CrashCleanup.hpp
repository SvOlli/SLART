/*
 * src/libs/Common/CrashCleanup.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef CRASHCLEANUP_HPP
#define CRASHCLEANUP_HPP CRASHCLEANUP_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QObject;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief small helper to clean up most important stuff by calling destructors
 of registered classes

 Registered objects will be deleted in reverse order of addition. Signal will
 be re-raised after completion.
*/
class CrashCleanup
{
public:
   /*!
    \brief add an object that will be deleted opon crash

    \param object object to add
   */
   static void addObject( QObject *object );

   /*!
    \brief remove a previously added object

    \param object object to remove
   */
   static void removeObject( QObject *object );

   /*!
    \brief the routine installed as the signal handler

    returns with exit( signal );
   */
   static void runCleanup( int signal );

private:
   CrashCleanup();
   virtual ~CrashCleanup();

   Q_DISABLE_COPY( CrashCleanup )

   static QList<QObject*> cObjects; /*!< \brief list of object to be deleted by runCleanup */
};

/*! @} */

#endif
