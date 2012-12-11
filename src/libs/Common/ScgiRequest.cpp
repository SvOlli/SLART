/*
 * src/libs/Common/ScgiRequest.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */


/* class declaration */
#include "ScgiRequest.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* class variables */


ScgiRequest::ScgiRequest( const QByteArray &raw )
: QHash<QByteArray,QByteArray>()
{
   bool ok;
   int colonPos = raw.indexOf(':');
   if( colonPos < 1 )
   {
      return;
   }
   int headerSize = QByteArray( raw.constData(), colonPos ).toInt( &ok );

   const char *c;
   const char *headerStart = raw.constData() + colonPos + 1;
   const char *headerEnd   = raw.constData() + colonPos + 1 + headerSize;
   const char *key = 0;
   const char *value = 0;
   int keySize = 0;
   int valueSize = 0;

   for( c = headerStart; c < headerEnd; ++c )
   {
      if( !key )
      {
         key = c;
         keySize = 0;
         continue;
      }

      if( !keySize && !*c )
      {
         keySize = c - key;
         continue;
      }

      if( keySize && !value )
      {
         value = c;
         continue;
      }

      if( value && !*c )
      {
         valueSize = c - value;

         insert( QByteArray::fromRawData( key, keySize ),
                 QByteArray::fromRawData( value, valueSize ) );

         key = 0;
         keySize = 0;
         value = 0;
         valueSize = 0;
      }
   }
   mpContentStart = headerEnd + 1;
   mContentSize = raw.size() - (mpContentStart - raw.constData());
}


const QByteArray ScgiRequest::content() const
{
   return QByteArray::fromRawData( mpContentStart, mContentSize );
}
