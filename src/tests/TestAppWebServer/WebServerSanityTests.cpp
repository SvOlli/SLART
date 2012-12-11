/*
 * src/tests/TestAppWebServer/WebServerSanityTests.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "WebServerSanityTests.hpp"

/* system headers */

/* Qt headers */

/* local library headers */
#include <ScgiRequest.hpp>

/* local headers */


static const char scgiRequestData[] =
      "70:"
      "CONTENT_LENGTH" "\0" "27" "\0"
      "SCGI" "\0" "1" "\0"
      "REQUEST_METHOD" "\0" "POST" "\0"
      "REQUEST_URI" "\0" "/deepthought" "\0"
      ","
      "What is the answer to life?";


int WebServerSanityTests::scgiRequest()
{
   QByteArray data;
   ScgiRequest scgiRequest( QByteArray::fromRawData(&scgiRequestData[0],sizeof(scgiRequestData)-1 ) );

   Q_ASSERT( scgiRequest.value("CONTENT_LENGTH") == "27" );
   Q_ASSERT( scgiRequest.value("SCGI") == "1" );
   Q_ASSERT( scgiRequest.value("REQUEST_METHOD") == "POST" );
   Q_ASSERT( scgiRequest.value("REQUEST_URI") == "/deepthought" );
   Q_ASSERT( scgiRequest.content() == "What is the answer to life?" );

   return 0;
}
