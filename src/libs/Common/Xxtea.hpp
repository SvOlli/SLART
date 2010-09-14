/**
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


class Xxtea
{
public:
   Xxtea();
   virtual ~Xxtea();
   /* set the key as 4 32-bit unsigned integer */
   void setKey( quint32 k0, quint32 k1, quint32 k2, quint32 k3 );
   /* set the key as a 16 byte array, returns true if data was valid and accepted */
   bool setKey( const QByteArray &key );
   /* get the key */
   QByteArray getKey();
   /* set the data to work upon, returns true if data was valid (needs to be a
      mutiple of 4 bytes) */
   bool setData( QByteArray *data );
   /* encode the data */
   bool encode();
   /* decode the data */
   bool decode();

private:
   const quint32  mDelta;
   quint32        *mpKey;
   QByteArray     *mpData;
};

#endif
