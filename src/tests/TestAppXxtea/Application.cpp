/**
 * src/tests/TestAppXxtea/Application.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the Lesser GNU Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QStringList>
#include <QTextStream>

/* local library headers */
#include <CommandLine.hpp>
#include <Xxtea.hpp>

/* local headers */


int main(int argc, char *argv[])
{
   int retval = 0;

   QCoreApplication::setOrganizationName("SLART");
   QCoreApplication::setOrganizationDomain("svolli.org");
   QCoreApplication::setApplicationName("TestAppXxtea");
   QCoreApplication app( argc, argv );

   QTextStream qStdOut( stdout );
   QTextStream qStdErr( stderr );
   QSettings   settings;

   bool help = false;
   bool encrypt = false;
   bool decrypt = false;
   QString key;
   QStringList files;
   CommandLine cl;
   cl.option( "-help",    "show help",          &help,    true );
   cl.option( "-encrypt", "boolean test set",   &encrypt, true );
   cl.option( "-decrypt", "boolean test unset", &decrypt, true );
   cl.option( "-setkey",  "string test",        &key );
   cl.parse( &files );

   if( cl.check() )
   {
      qStdErr << "fail! try '-help'\n";
      return 0;
   }

   if( help )
   {
      qStdErr
      << "options:\n"
      << "-encrypt, -decrypt: encrypt or decrypt files (you need to specify one of those)\n"
      << "-setkey <key>: set the key to use (cached in registry, key will be md5'ed)\n"
      ;
      return 0;
   }

   if( key.isNull() )
   {
      key = settings.value( "Key" ).toByteArray();
   }
   else
   {
      if( key.isEmpty() )
      {
         settings.remove( "Key" );
         qStdOut << "deleting key\n";
      }
      else
      {
         QCryptographicHash hash( QCryptographicHash::Md5 );
         hash.addData( key.toUtf8() );
         settings.setValue( "Key", hash.result() );
         qStdOut << "setting key to: " << settings.value( "Key" ).toByteArray().toHex() << "\n";
      }

      if( !encrypt && !decrypt )
      {
         return 0;
      }
   }

   if( encrypt == decrypt )
   {
      qStdErr
      << "you need to specify exactly one of -encrypt or -decrypt\n"
      ;
   }

   QFile       file;
   QByteArray  fileData;
   Xxtea       xxtea;
   bool        success;

   xxtea.setKey( settings.value( "Key" ).toByteArray() );
   xxtea.setData( &fileData );
   for( int i = 0; i < files.size(); i++ )
   {
      qStdOut << files.at(i) << ": ";
      file.setFileName( files.at(i) );
      file.open( QIODevice::ReadWrite );
      qStdOut << "reading, ";
      fileData = file.readAll();
      if( encrypt )
      {
         qStdOut << "encrypting, ";
         success = xxtea.encode();
      }
      if( decrypt )
      {
         qStdOut << "decrypting, ";
         success = xxtea.decode();
      }
      if( !success )
      {
         qStdOut << "not all data could be processed, ";
      }
      file.seek( 0 );
      qStdOut << "writing, ";
      file.write( fileData );
      file.close();
      qStdOut << "done.\n";
   }

   return retval;
}
