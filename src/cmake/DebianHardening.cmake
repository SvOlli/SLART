#
# src/cmake/DebianHarden.cmake
#
# written by SvOlli
# distributed as public domain
#
# for the intension of this script take a look at http://wiki.debian.org/Hardening
#

FIND_PROGRAM( dpkg_buildflags NAMES dpkg-buildflags )
IF( dpkg_buildflags )
   EXECUTE_PROCESS( COMMAND ${dpkg_buildflags} --get CPPFLAGS
                    OUTPUT_VARIABLE dpkg_buildflags_cpp
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
   EXECUTE_PROCESS( COMMAND ${dpkg_buildflags} --get CFLAGS
                    OUTPUT_VARIABLE dpkg_buildflags_c
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
   EXECUTE_PROCESS( COMMAND ${dpkg_buildflags} --get CXXFLAGS
                    OUTPUT_VARIABLE dpkg_buildflags_cxx
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
   EXECUTE_PROCESS( COMMAND ${dpkg_buildflags} --get LDFLAGS
                    OUTPUT_VARIABLE dpkg_buildflags_ld
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
   SET( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${dpkg_buildflags_cpp} ${dpkg_buildflags_c}" )
   SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${dpkg_buildflags_cpp} ${dpkg_buildflags_cxx}" )
   SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${dpkg_buildflags_ld}" )
   MESSAGE( STATUS "Enabling Debian Hardening. Flags added:" )
   MESSAGE( STATUS "  C Compiler: ${dpkg_buildflags_cpp} ${dpkg_buildflags_c}" )
   MESSAGE( STATUS "  C++ Compiler: ${dpkg_buildflags_cpp} ${dpkg_buildflags_cxx}" )
   MESSAGE( STATUS "  Linker: ${dpkg_buildflags_ld}" )
ENDIF( dpkg_buildflags )
