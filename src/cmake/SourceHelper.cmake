#
# src/cmake/SourceHelper.cmake
#
# written by SvOlli
# distributed as public domain
#

MACRO( ADD_CLASS _HEADER _SOURCE _CLASS )

   SET( ${_HEADER} ${${_HEADER}} ${_CLASS}.hpp )
   SET( ${_SOURCE} ${${_SOURCE}} ${_CLASS}.cpp )
   MESSAGE( "Header: ${_HEADER} => ${${_HEADER}}" )
   MESSAGE( "Source: ${_SOURCE} => ${${_SOURCE}}" )
   
ENDMACRO( ADD_CLASS )

MACRO( ADD_CLASS1 _CLASS )

   IF( NOT DEFINED HEADERS )
      MESSAGE( FATAL_ERROR "Meta-Variable 'HEADERS' not set" )
   ENDIF( NOT DEFINED HEADERS )
   IF( NOT DEFINED SOURCES )
      MESSAGE( FATAL_ERROR "Meta-Variable 'SOURCES' not set" )
   ENDIF( NOT DEFINED SOURCES )
   SET( ${HEADERS} ${${HEADERS}} ${_CLASS}.hpp )
   SET( ${SOURCES} ${${SOURCES}} ${_CLASS}.cpp )

ENDMACRO( ADD_CLASS1 _CLASS )
