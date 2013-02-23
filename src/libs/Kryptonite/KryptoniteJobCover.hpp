/*
 * src/libs/Kryptonite/KryptoniteJobCover.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef KRYPTONITEJOBCOVER_HPP
#define KRYPTONITEJOBCOVER_HPP KRYPTONITEJOBCOVER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QString;
class QUrl;

/* forward declaration of local classes */
class Kryptonite;


/*!
  \addtogroup Kryptonite

  @{
*/

/*!
 \brief \todo complete documentation

*/
class KryptoniteJobCover : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

   */
   KryptoniteJobCover( Kryptonite *kryptonite, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~KryptoniteJobCover();

   static void requestImage( KryptoniteJobCover *job,
                             const QUrl &url, const QString &fileName = QString() );

public slots:
   virtual void requestList( const QString &query ) = 0;
   virtual void requestImage( const QUrl &url, const QString &fileName = QString() ) = 0;

signals:
   /*!
    \brief internally used for communication with Kryptonite

    \param object
    \param slot
    \param urlList
    \param payload
   */
   void requestDownload( QObject *object, const char *slot,
                         const QUrl &url, const QVariant &payload = QVariant() );
   void imageFound( const QByteArray &preview, const QVariant &link );
   void imageDownloaded( const QByteArray &data, const QVariant &fileName );

private:
};

/*! @} */

#endif
