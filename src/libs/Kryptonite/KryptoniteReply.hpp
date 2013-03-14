/*
 * src/libs/Kryptonite/KryptoniteReply.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef KRYPTONITEREPLY_HPP
#define KRYPTONITEREPLY_HPP KRYPTONITEREPLY_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QNetworkRequest>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QNetworkAccessManager;
class QNetworkReply;

/* forward declaration of local classes */


/*!
  \addtogroup Kryptonite

  @{
*/

/*!
 \brief this class is only used internally by \ref Kryptonite

 \dotfile "graphs/libs/Kryptonite/KryptoniteReply_connect.dot" "Connect Graph"
*/
class KryptoniteReply : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param manager network access manager used to create network connection
    \param request request to send
    \param target target object to send fetched data to
    \param slot target slot to send fetched data to
    \param payload payload to pass on to target
   */
   KryptoniteReply( QNetworkAccessManager *manager,
                    const QNetworkRequest &request,
                    QObject *target, const char *slot,
                    const QVariant &payload = QVariant() );

   /*!
    \brief destructor

    will be called internally after completion
   */
   virtual ~KryptoniteReply();

public slots:
   /*!
    \brief internally used to send out the request in a delayed fashion

   */
   void request();

   /*!
    \brief internally used to get data when reply is finished

   */
   void getData();

signals:
   /*!
    \brief internally used to send back data to target specified at constructor

    \param data data fetched from server
    \param payload payload stored at constructor
   */
   void complete( const QByteArray &data, const QVariant &payload );

private:
   Q_DISABLE_COPY( KryptoniteReply )

   QNetworkAccessManager   *mpManager; /*!< \brief network access manager to use */
   QNetworkReply           *mpReply; /*!< \brief reply stored to obtain data */
   const QNetworkRequest   mRequest; /*!< \brief request stored for delayed invocation */
   const QVariant          mPayload; /*!< \brief payload to pass on to target */
};

/*! @} */

#endif
