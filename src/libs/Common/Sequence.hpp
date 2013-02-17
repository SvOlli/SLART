/*
 * src/libs/Common/Sequence.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP SEQUENCE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QMutex>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common
  \brief a generator of a sequention number

  most commonly used with \ref SingleInstance
  @{
  */


class Sequence : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

   */
   Sequence( QObject *parent = 0 );
   /*!
    \brief constructor

   */
   Sequence( unsigned int start, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~Sequence();

   /*!
    \brief get the next number

   */
   unsigned int get();

   /*!
    \brief for convienience a global sequence

    using \ref SingleInstance

    \return unsigned int
   */
   static unsigned int global();

signals:
   /*!
    \brief signaling that a new

    \param n new number fetched with get
   */
   void current( unsigned int n );

private:
   Q_DISABLE_COPY( Sequence )

   unsigned int   mSequence;
   QMutex         mMutex;
};

/*! @} */

#endif /* SEQUENCE_HPP */
