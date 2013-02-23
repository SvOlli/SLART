/*
 * src/libs/Kryptonite/KryptoniteJobCoverAmazonDE.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef KRYPTONITEJOBCOVERAMAZONDE_HPP
#define KRYPTONITEJOBCOVERAMAZONDE_HPP KRYPTONITEJOBCOVERAMAZONDE_HPP

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

class KryptoniteJobCoverAmazonDE : public KryptoniteJobCover
{
   Q_OBJECT

public:
   KryptoniteJobCoverAmazonDE( Kryptonite *kryptonite,
                               QObject *parent = 0 );
   virtual ~KryptoniteJobCoverAmazonDE();

public slots:
   void requestList( const QString &query );
   void requestImage( const QUrl &url, const QString &fileName = QString() );

   void parseListHtml( const QByteArray &data );
   void parseItemHtml( const QByteArray &data, const QVariant &payload );

signals:

private:
   Q_DISABLE_COPY( KryptoniteJobCoverAmazonDE )
};

#endif
