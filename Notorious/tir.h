/**
 * tir.h
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 *
 * tir is a small "unTar In Ram" library that right now only unpacks tar.bz2
 */


#ifndef TIR_H
#define TIR_H TIR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
   void   *fp;
   char   *filename;
   char   *filetype;
   size_t filesize;
   void   *header;
   char   *data;
   size_t datasize;
} tir_data;


int tir_open( const char *filename, tir_data *handle );
int tir_read( tir_data *handle );
int tir_close( tir_data *handle );

#ifdef __cplusplus
}
#endif

#endif
