/*
 * src/libs/Common/MagicEncoderLoader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERLOADER_HPP
#define MAGICENCODERLOADER_HPP MAGICENCODERLOADER_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QApplication;
class QDir;
class QPluginLoader;

/* forward declaration of local classes */
class MagicEncoderProxy;
class Satellite;
typedef QList<MagicEncoderProxy*> MagicEncoderList;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class MagicEncoderLoader
{
public:
   /*!
    \brief function to load MagicEncoders

   */
   static MagicEncoderList tryLoading( const QString &msgHeader,
                                       Satellite *satellite = 0 );

private:

   /*!
    \brief constructor

   */
   MagicEncoderLoader();

   /*!
    \brief destructor

   */
   virtual ~MagicEncoderLoader();

   Q_DISABLE_COPY( MagicEncoderLoader )

   /*!
    \brief internal function to load MagicEncoders

   */
   static MagicEncoderList tryLoading( const QString &msgHeader, const QDir &dir,
                                       Satellite *satellite = 0 );
};

/*! @} */

#endif
