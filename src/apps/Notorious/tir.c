/**
 * src/apps/Notorious/tir.c
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 *
 * tir is a small "unTar In Ram" library that right now only unpacks tar.bz2
 */


/* declarations */
#include "tir.h"

/* system headers */
#include <stdlib.h>
#include <string.h>
#include <bzlib.h>


#define TAR_BLOCK_SIZE 0x00000200
#define TAR_BLOCK_MASK 0xfffffe00


/*
 * TODO:
 * - handle short reads on header
 * - handle short reads on data
 * - create better error reporting
 */


int tir_open( const char *filename, tir_data *handle )
{
   memset( handle, 0, sizeof( tir_data ) );
   handle->fp = (void*)BZ2_bzopen( filename, "rb" );
   if( !(handle->fp) )
   {
      return 1;
   }

   handle->header = malloc( TAR_BLOCK_SIZE );
   return 0;
}


int tir_read( tir_data *handle )
{
   int bytesread = 0;
   char *number = 0;

   bytesread = BZ2_bzread( (BZFILE*)(handle->fp), handle->header, TAR_BLOCK_SIZE );
   if( bytesread != TAR_BLOCK_SIZE )
   {
      return 1;
   }

   handle->filename = (char*)(handle->header);
   handle->filetype = (char*)(handle->header) + 156;
   number = (char*)(handle->header) + 124;
   handle->filesize = (size_t)strtoul( number, NULL, 8 );

   //printf( "filesize=%d,datasize=%d\n", (int)handle->filesize, (int)handle->datasize );
   if( handle->filesize > handle->datasize )
   {
      handle->datasize = (handle->filesize & TAR_BLOCK_MASK) + TAR_BLOCK_SIZE;
      handle->data = (char*)realloc( handle->data, handle->datasize );
   }

   if( handle->filesize )
   {
      long int readsize = handle->filesize;
      if( handle->filesize % TAR_BLOCK_SIZE )
      {
         readsize = (readsize & TAR_BLOCK_MASK) + TAR_BLOCK_SIZE;
      }
      bytesread = BZ2_bzread( (BZFILE*)(handle->fp), handle->data, readsize );
      if( bytesread != readsize )
      {
         //printf( "shortread: %d < %ld\n", bytesread, readsize );
         return 2;
      }
   }

   return 0;
}


int tir_close( tir_data *handle )
{
   if( handle->header )
   {
      free( handle->header );
      handle->header = 0;
   }

   if( handle->data )
   {
      free( handle->data );
      handle->data = 0;
      handle->datasize = 0;
   }

   BZ2_bzclose( (BZFILE*)(handle->fp) );

   return 0;
}
