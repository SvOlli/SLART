/*
 * src/apps/Notorious/tir.h
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */


#ifndef TIR_H
#define TIR_H TIR_H

/*!
  \addtogroup Notorious

  @{
*/

/*!
 \brief "unTar In Ram"

 A small set of routines to handle reading data from a tar archive.
 Right now only tar.bz2 is supported. The files contained in the tar archive
 will be read in RAM one by one, the complete payload data of the file will be
 available as an allocated memory block, so be aware the reading tars with
 large entries, as it will blow at least the current process.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

/*!
 \brief struct to handle all information about the tar archive

*/
typedef struct tir_data
{
   void   *fp; /*!< \brief filepointer */
   char   *filename; /*!< \brief name of the current archive entry */
   char   *filetype; /*!< \brief type of the current archive entry */
   size_t filesize; /*!< \brief size of file */
   void   *header; /*!< \brief pointer to the raw header data */
   char   *data; /*!< \brief pointer to the raw file payload */
   size_t datasize; /*!< \brief size of the data */
} tir_data;


/*!
 \brief open a tar archive for reading

 \param filename the name of the tar archive
 \param handle the handle to fill all necessary data in
 \return int 0: success, 1: fail
*/
int tir_open( const char *filename, tir_data *handle );

/*!
 \brief read the next file entry

 \param handle the handle read the next file in
 \return int 0: success, 1: next header could not be read, probably eof,
   2: error during the reading of the payload
*/
int tir_read( tir_data *handle );

/*!
 \brief close the tar archive

 \param handle the handle of the tar archive to close
 \return int 0: success
*/
int tir_close( tir_data *handle );

#ifdef __cplusplus
}
#endif

/*! @} */

#endif
