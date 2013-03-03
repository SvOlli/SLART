/*
 * src/libs/Kryptonite/KryptoniteJobCoverDiscogs.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef KRYPTONITEJOBCOVERDISCOGS_HPP
#define KRYPTONITEJOBCOVERDISCOGS_HPP KRYPTONITEJOBCOVERDISCOGS_HPP

/* base class */
#include "KryptoniteJobCover.hpp"

/* system headers */

/* Qt headers */
#include <QStringList>
#include <QUrl>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QString;

/* forward declaration of local classes */
class Kryptonite;


/*!
  \addtogroup Kryptonite

  @{
*/

/*!
 \brief class for downloading cover art image from amazon.de

 \dotfile "graphs/libs/Common/KryptoniteCoverAmazonDE_connect.dot" "Connect Graph"
*/
class KryptoniteJobCoverDiscogs : public KryptoniteJobCover
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param kryptonite Kryptonite to use for download
    \param parent
   */
   KryptoniteJobCoverDiscogs( Kryptonite *kryptonite, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~KryptoniteJobCoverDiscogs();

public slots:
   /*!
    \brief api call for searching

    \param query what to search
   */
   void requestList( const QString &query );
   /*!
    \brief api to downloading one image

    \param url as obtained by imageFound
    \param payload to send back via imageDownloaded
   */
   void requestImage( const QUrl &url, const QVariant &payload = QVariant() );

   /*!
    \brief internally used as callback for downloaded data

    \param data search html to parse
   */
   void parseListHtml( const QByteArray &data );
   /*!
    \brief internally used as callback for downloaded data

    \param data item html to parse
    \param payload data to send back to invoker
   */
   void parseItemHtml( const QByteArray &data, const QVariant &payload );
   /*!
    \brief internally used as callback for downloaded data

    \param data item html to parse
    \param payload data to send back to invoker
   */
   void parseImagesHtml( const QByteArray &data, const QVariant &payload );

signals:

private:
   Q_DISABLE_COPY( KryptoniteJobCoverDiscogs )
};

#endif
