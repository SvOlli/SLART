/*
 * src/libs/Common/Xxtea.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "Xxtea.hpp"

/* system headers */

/* Qt headers */
#include <QtEndian>
#include <QByteArray>

/* local library headers */

/* local headers */

/* class variables */


#define MX ((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (mpKey[(p&3)^e] ^ z));

Xxtea::Xxtea()
: mDelta( 0x9e3779b9 )
, mpKey( new quint32[4] )
, mpData( 0 )
{
}

Xxtea::~Xxtea()
{
   delete[] mpKey;
}

void Xxtea::setKey( quint32 k0, quint32 k1, quint32 k2, quint32 k3 )
{
   mpKey[0] = qToBigEndian( k0 );
   mpKey[1] = qToBigEndian( k1 );
   mpKey[2] = qToBigEndian( k2 );
   mpKey[3] = qToBigEndian( k3 );
}


bool Xxtea::setKey( const QByteArray &key )
{
   if( key.size() != 16 )
   {
      return false;
   }

   quint32 *k = (quint32*)key.data();
   for( int i = 0; i < 4; i++ )
   {
      mpKey[i] = qToBigEndian( k[i] );
   }
   return true;
}


QByteArray Xxtea::getKey()
{
   QByteArray key( 16, 0 );

   quint32 *k = (quint32*)key.data();
   for( int i = 0; i < 4; i++ )
   {
      k[i] = qFromBigEndian( mpKey[i] );
   }

   return key;
}


bool Xxtea::setData( QByteArray *data )
{
   mpData = data;
   if( !data )
   {
      return false;
   }
   return (mpData->size() & 3) == 0;
}


bool Xxtea::encode()
{
   quint32 *v = (quint32*)mpData->data();
   quint32 n = mpData->size() / sizeof(quint32);
   quint32 rounds = 6 + 52/n;
   quint32 sum = 0;
   quint32 z = v[n-1];
   quint32 y = 0;
   quint32 p = 0;
   quint32 e = 0;

   if( n < 2 )
   {
      return false;
   }

   do
   {
      sum += mDelta;
      e = (sum >> 2) & 3;
      for( p = 0; p < n-1; p++ )
      {
         y = v[p+1], z = v[p] += MX;
      }
      y = v[0];
      z = v[n-1] += MX;
   }
   while( --rounds );

   return (mpData->size() & 3) == 0;
}


bool Xxtea::decode()
{
   quint32 *v = (quint32*)mpData->data();
   quint32 n = mpData->size() / sizeof(quint32);
   quint32 rounds = 6 + 52/n;
   quint32 sum = rounds*mDelta;
   quint32 y = v[0];
   quint32 z = 0;
   quint32 p = 0;
   quint32 e = 0;

   if( n < 2 )
   {
      return false;
   }

   do
   {
      e = (sum >> 2) & 3;
      for( p = n-1; p > 0; p-- )
      {
         z = v[p-1], y = v[p] -= MX;
      }
      z = v[n-1];
      y = v[0] -= MX;
   }
   while( sum -= mDelta );

   return (mpData->size() & 3) == 0;
}

