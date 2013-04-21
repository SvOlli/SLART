/*
 * QVariantReply.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef QVARIANTREPLY_HPP
#define QVARIANTREPLY_HPP QVARIANTREPLY_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief small helper class for sending async data

 Here's a small example for usage.

 Master:
 \code
 class Master : public QObject
 {
 // [...]
 public:
    void createJob()
    {
       mpWorker->startJob( this, SLOT(handleReply(QVariant)) );
    }
 // [...]
 public slots:
    void handleReply( const QVariant &data )
    {
       // ... do something with data ...
    }
 // [...]
 private:
    class Worker  *mpWorker;
 // [...]
 }
 \endcode

 Worker:
 \code
 class Worker : public QObject
 {
 // [...]
 public:
    void createJob( QObject *target, const char *method )
    {
       QVariantReply *reply = new QVariantReply( target, method );
       QVariant replyData;
       // ... do the work ...
       reply->send( replyData );
    }
 // [...]
 }
 \endcode

*/
class QVariantReply : public QObject
{
   Q_OBJECT
   
public:
   /*!
    \brief creator

    \param target target object
    \param method target method which needs to match parameters QVariant
   */
   QVariantReply( QObject *target, const char *method );
   /*!
    \brief destructor

   */
   virtual ~QVariantReply();

signals:
   /*!
    \brief internally used to send the data

    \internal
    \param data
   */
   void sendReply( const QVariant &data );

public slots:
   /*!
    \brief send data to registered target and delete this object afterwards

    \param data
   */
   void send( const QVariant &data );

private:
   Q_DISABLE_COPY( QVariantReply )
};

//! @}

#endif
