#
# src/libs/Magic/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Magic/MagicInterface ====" )

PROJECT( MagicInterface )

SET( MagicInterface_PUBLIC_HEADERS
   MagicEncoderInterface.hpp
)

#ADD_LIBRARY( MagicInterface
#   ${MagicInterface_SOURCES)
#   ${MagicInterface_PUBLIC_HEADERS}
#)

SET( MAGICINTERFACE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "MagicInterface include directory" )

MESSAGE( STATUS "propagating local interface MagicInterface" )
MESSAGE( STATUS "  include dir: MAGICINTERFACE_INCLUDE_DIR=${MAGICINTERFACE_INCLUDE_DIR}" )

ADD_SUBDIRECTORY( MagicEncoder )
ADD_SUBDIRECTORY( MagicEncoderFlac )
ADD_SUBDIRECTORY( MagicEncoderMp3 )
ADD_SUBDIRECTORY( MagicEncoderOgg )
ADD_SUBDIRECTORY( MagicEncoderWav )
