#
# src/cmake/FindLame.cmake
#
# written by SvOlli
# distributed as public domain
#

FIND_PATH( LAME_INCLUDE_DIR
   NAMES lame/lame.h
   DOC "lame (mp3 encoder) include directory"
)

FIND_LIBRARY( LAME_LIBRARY
   NAMES mp3lame
   DOC "lame (mp3 encoder) library"
)

# handle the QUIETLY and REQUIRED arguments and set LAME_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LAME
   REQUIRED_VARS LAME_LIBRARY LAME_INCLUDE_DIR
)

IF( LAME_FOUND )
   SET( LAME_LIBRARIES ${LAME_LIBRARY} )
   SET( LAME_INCLUDE_DIRS ${LAME_INCLUDE_DIR} )
ENDIF()

MARK_AS_ADVANCED( LAME_INCLUDE_DIR LAME_LIBRARY )
