/*
 * src/tests/TestAppWebServer/Resources/ajax.js
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */


var http = new XMLHttpRequest();

function loadFields()
{
   http.open( 'GET', '/get', true );
   http.onreadystatechange = parseFields;
   http.send( null );
}

function parseFields()
{
   if( http.readyState == 4 )
   {
      if (http.status == 200)
      {
         var reply  = http.responseXML.getElementsByTagName("reply")[0].firstChild.nodeValue;
         var artist = http.responseXML.getElementsByTagName("artist")[0].firstChild.nodeValue;
         var title  = http.responseXML.getElementsByTagName("title")[0].firstChild.nodeValue;
         var album  = http.responseXML.getElementsByTagName("album")[0].firstChild.nodeValue;
         var track  = http.responseXML.getElementsByTagName("track")[0].firstChild.nodeValue;

         document.getElementsByName("status")[0].value = 'connected';
         document.getElementsByName("reply")[0].value  = reply;
         document.getElementsByName("artist")[0].value = artist;
         document.getElementsByName("title")[0].value  = title;
         document.getElementsByName("album")[0].value  = album;
         document.getElementsByName("track")[0].value  = track;

         http.open( 'GET', '/wait', true );
         http.onreadystatechange = parseWait;
         http.send( null );
      }
      else
      {
         document.getElementsByName("status")[0].value = 'disconnected';
         window.setTimeout( 'loadFields();', 15000 );
      }
   }
}

function parseWait()
{
   if( http.readyState == 4 )
   {
      loadFields();
   }
}

loadFields();
