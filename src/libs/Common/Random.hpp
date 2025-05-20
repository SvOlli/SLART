/*
 * src/libs/Common/Random.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef RANDOM_HPP
#define RANDOM_HPP RANDOM_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QRandomGenerator>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Common
  @{
  */

/*!
 \brief abstraction layer

 \dotfile "graphs/libs/Common/Random_connect.dot" "Connect Graph"
*/
class Random : public QObject
{
   Q_OBJECT

public slots:

public:
   /*!
    \brief get QSettings pointer according to application name

    \param applicationName
    \return
   */
   static QRandomGenerator *get();

   /*!
    \brief TODO!

    \param max
    \return
   */
   static quint32 bounded( quint32 max );

   /*!
    \brief TODO!

    \return
   */
   static quint32 generate();

private:
   /*!
    \brief constructor

    \param parent parent QObject
   */
   Random( QObject *parent = QCoreApplication::instance() );

   /*!
    \brief destructor

   */
   virtual ~Random();

   QRandomGenerator           mRandom;
   static Random              *cpRandom; /*!< handle for static functions */
};

/*! @} */

#endif
