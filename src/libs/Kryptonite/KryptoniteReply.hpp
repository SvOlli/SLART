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
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QNetworkReply;

/* forward declaration of local classes */


/*!
  \addtogroup Kryptonite

  @{
*/

/*!
 \brief this class is only used internally by \ref Downloader

 \dotfile "graphs/libs/Common/KryptoniteReply_connect.dot" "Connect Graph"
*/
class KryptoniteReply : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param reply network reply
    \param target target object to send fetched data to
    \param slot target slot to send fetched data to
    \param payload payload to pass on to target
   */
   KryptoniteReply( QNetworkReply *reply,
                   QObject *target, const char *slot,
                   const QVariant &payload = QVariant() );
   /*!
    \brief destructor

    will be called internally after completion
   */
   virtual ~KryptoniteReply();

   static void async( QObject* object, const char *slot,
                      const QByteArray &data, const QVariant &payload = QVariant() );
public slots:
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

   QNetworkReply        *mpReply; /*!< \brief reply stored to obtain data */
   const QVariant       mPayload; /*!< \brief payload to pass on to target */
};

#endif
