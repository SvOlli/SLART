
#ifndef TIR_H
#define TIR_H TIR_H

#ifdef __cplusplus
extern "C" {
#endif

#define TIR_OPEN_MODE_TAR 0
#define TIR_OPEN_MODE_GZ  1
#define TIR_OPEN_MODE_BZ2 2

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
