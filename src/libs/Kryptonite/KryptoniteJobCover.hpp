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
 \brief base class for downloading cover art image

 \dotfile "graphs/libs/Common/KryptoniteJobCover_connect.dot" "Connect Graph"
*/
class KryptoniteJobCover : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param kryptonite Kryptonite to use for download
    \param parent
   */
   KryptoniteJobCover( Kryptonite *kryptonite, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~KryptoniteJobCover();

   /*!
    \brief api to downloading one image, wrapper for requestImage slot

    \param job pointer of object to invoke
    \param url as obtained by imageFound
    \param fileName to send to imageDownloaded
   */
   static void requestImage( KryptoniteJobCover *job,
                             const QUrl &url, const QString &fileName = QString() );

public slots:
   /*!
    \brief api call for searching

    \param query what to search
   */
   virtual void requestList( const QString &query ) = 0;
   /*!
    \brief api to downloading one image

    \param url as obtained by imageFound
    \param fileName to send to imageDownloaded
   */
   virtual void requestImage( const QUrl &url, const QString &fileName = QString() ) = 0;

signals:
   /*!
    \brief internally used for communication with Kryptonite

    \param object handler for downloaded data (usually "this")
    \param slot slot of the data handler
    \param url url to download
    \param payload payload to pass on to callback
   */
   void requestDownload( QObject *object, const char *slot,
                         const QUrl &url, const QVariant &payload = QVariant() );
   /*!
    \brief used by subclasses to signalize that a new cover preview has been obtained

    \param preview image data as downloaded for display
    \param link link of the page containing the full size image to pass via
   */
   void imageFound( const QByteArray &preview, const QVariant &link );
   /*!
    \brief used by subclasses to signalize that the selected cover has been obtained

    \param data image data as downloaded for display or saving
    \param fileName as been passed by requestImage
   */
   void imageDownloaded( const QByteArray &data, const QVariant &fileName );

private:
};

/*! @} */

#endif
