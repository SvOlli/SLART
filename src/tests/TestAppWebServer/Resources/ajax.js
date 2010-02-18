/**
 * src/tests/TestAppWebServer/Resources/ajax.js
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the Lesser GNU Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */


http = new XMLHttpRequest();
http.open( 'GET', '/wait', true );
http.onreadystatechange = useHttpResponse;
http.send(null);

function useHttpResponse()
{
   if( http.readyState == 4 )
   {
      window.location.reload();
   }
}
