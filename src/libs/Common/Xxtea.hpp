/*
 * src/libs/Common/Xxtea.hpp
 * written by Sven Oliver Moll
 *
 * implementation of the XXTEA encryption algorithm
 * see http://en.wikipedia.org/wiki/XXTEA for details
 * note that this cipher is considered broken since May 2010
 * when you encode 2**59 plain texts you can calculate the keys
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef XXTEA_HPP
#define XXTEA_HPP XXTEA_HPP

/* system headers */

/* Qt headers */
#include <QtGlobal>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QByteArray;

/* forward declaration of local classes */


/*!
  \addtogroup Common
 @{
 */

/*!
 \brief Encrypt data using the xxtea algorithm. If data size is less then 8 bytes no encryption will take place.
 If data size is not a multiple of 4 bytes, the last not fitting bytes will not encrypted.

*/
class Xxtea
{
public:
   /*!
    \brief constructor

   */
   Xxtea();
   /*!
    \brief destructor

   */
   virtual ~Xxtea();

   /*!
    \brief set the key as 4 32-bit unsigned integer

    \param k0
    \param k1
    \param k2
    \param k3
   */
   void setKey( quint32 k0, quint32 k1, quint32 k2, quint32 k3 );
   /*!
    \brief set the key as a 16 byte array, returns true if data was valid and accepted

    \param key
   */
   bool setKey( const QByteArray &key );
   /*!
    \brief get the key

   */
   QByteArray getKey();
   /*!
    \brief set the data to work upon, returns true if data was valid (needs to be a
      mutiple of 4 bytes, returns )

    \return false if data isn't a multiple of 4 bytes
    \param data
   */
   bool setData( QByteArray *data );

   /*!
    \brief encode the data

    \return false if data isn't a multiple of 4 bytes
   */
   bool encode();

   /*!
    \brief decode the data

    \return false if data isn't a multiple of 4 bytes
   */
   bool decode();

private:
   Q_DISABLE_COPY( Xxtea )

   const quint32  mDelta; /*!< "delta" from algorithm */
   quint32        *mpKey; /*!< array of key components */
   QByteArray     *mpData; /*!< pointer to the data */
};

/*! @} */

#endif
