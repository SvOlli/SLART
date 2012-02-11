/*
 * src/tests/TestAppXxtea/Application.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* system headers */
#include <cstdio>

/* Qt headers */
#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
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

   QString progName( QFileInfo( QCoreApplication::arguments().at(0) ).fileName() );
   bool help = false;
   bool encrypt = false;
   bool decrypt = false;
   QString key;
   QStringList files;
   CommandLine cl;
   cl.option( "-help",    "show help",     &help,    true );
   cl.option( "-encrypt", "encrypt files", &encrypt, true );
   cl.option( "-decrypt", "decrypt files", &decrypt, true );
   cl.option( "-key",     "set key",       &key );
   cl.parse( &files );

   if( cl.check() )
   {
      qStdErr << "fail! try '-help'\n";
      return 0;
   }

   if( help )
   {
      qStdErr <<
      "\nthis program encrypts and decrypts files using the xxtea algorithm\n"
      "see http://en.wikipedia.org/wiki/XXTEA for details\n"
      "\noptions:\n" << cl.help() << "\n\nexamples:\n"
      << progName << " -key secret                # writes the key secret in the registry\n"
      << progName << " -encrypt file1 (file2 ...) # encryptes the files\n"
      << progName << " -decrypt file1 (file2 ...) # decryptes the files\n"
      << progName << " -key secret -encrypt file  # set key and encrypt in one pass\n\n"
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
      qStdErr << "you need to specify exactly one of -encrypt or -decrypt\n";
      return 1;
   }

   QFile       file;
   QByteArray  fileData;
   Xxtea       xxtea;
   bool        success = false;

   QByteArray hashedKey( settings.value( "Key" ).toByteArray() );
   if( hashedKey.size() != 16 )
   {
      qStdOut << "key is invalid\n";
      return 1;
   }
   else
   {
      qStdOut << "key is: " << hashedKey.toHex() << "\n";
   }
   xxtea.setKey( hashedKey );
   xxtea.setData( &fileData );
   foreach( const QString &fileName, files )
   {
      qStdOut << fileName << ": ";
      file.setFileName( fileName );
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
         qStdOut << "some or all data could not be processed, ";
      }
      file.seek( 0 );
      qStdOut << "writing, ";
      file.write( fileData );
      file.close();
      qStdOut << "done.\n";
   }

   return retval;
}
