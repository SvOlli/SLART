#
# src/cmake/SetVersion.cmake
#
# written by SvOlli
# distributed as public domain
#

SET( MAJOR_VERSION 0 )
SET( MINOR_VERSION 8 )
SET( PATCH_VERSION 0 )
SET( SLART_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} )
ADD_DEFINITIONS( -DSLART_VERSION=\"${SLART_VERSION}\" )
MESSAGE( STATUS "====================" )
MESSAGE( STATUS "SLART version: ${SLART_VERSION}")
MESSAGE( STATUS "====================" )

IF( IS_DIRECTORY "${PROJECT_SOURCE_DIR}/../.git" )
   FIND_PROGRAM( git NAMES git )
   IF( git )
      EXECUTE_PROCESS( COMMAND ${PROJECT_SOURCE_DIR}/../extra/git-revision.sh
                       OUTPUT_VARIABLE SLART_REVISION
                       OUTPUT_STRIP_TRAILING_WHITESPACE )
      MESSAGE( STATUS "Revision is: ${SLART_REVISION}" )
   ELSE( git )
      MESSAGE( STATUS "git not found" )
   ENDIF( git )
ELSE( IS_DIRECTORY "${PROJECT_SOURCE_DIR}/../.git" )
   MESSAGE( STATUS "${PROJECT_SOURCE_DIR}/../.git not found" )
ENDIF( IS_DIRECTORY "${PROJECT_SOURCE_DIR}/../.git" )
