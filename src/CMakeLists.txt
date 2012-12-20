#
# src/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

IF( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
   MESSAGE( FATAL_ERROR "Only out of sources builds are supported" )
ENDIF( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )

MESSAGE( STATUS "==== src ====" )

SET( CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)
INCLUDE( SetVersion )

#SET( MAJOR_VERSION 0 )
#SET( MINOR_VERSION 8 )
#SET( PATCH_VERSION 0 )
#SET( SLART_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} )
#ADD_DEFINITIONS( -DSLART_VERSION=\"${SLART_VERSION}\" )
#MESSAGE( STATUS "====================" )
#MESSAGE( STATUS "SLART version: ${SLART_VERSION}")
#MESSAGE( STATUS "====================" )

#SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
#CMAKE_POLICY(SET CMP0003 OLD)

IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release )
ENDIF( NOT CMAKE_BUILD_TYPE )

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   MESSAGE( STATUS "Debug build" )
   ADD_DEFINITIONS( -DDEBUG )
   SET( CMAKE_VERBOSE_MAKEFILE true )
   SET( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/root" )
ELSE( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   MESSAGE( STATUS "non-debug build" )
   INCLUDE( DebianHardening )
   ADD_DEFINITIONS( -ffunction-sections -fdata-sections -DNDEBUG )
   SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections" )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )

IF( NOT CMAKE_INSTALL_PREFIX )
   MESSAGE( FATAL_ERROR "CMAKE_INSTALL_PREFIX is not set but it has to be. Something is wrong." )
ENDIF()
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )

#LINK_DIRECTORIES( /usr/local/lib/ ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )

ADD_DEFINITIONS( -Wall )
INCLUDE( FindPkgConfig )
FIND_PACKAGE( Qt4 4.6.0 REQUIRED )
MESSAGE( STATUS "Qt Edition: ${QT_EDITION}" )
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES(
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
)

# additional cmake files
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake )

# This allows to install and read the configs from non-standard locations
#ADD_DEFINITIONS(-DSHARE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/razor/\")

########### Add uninstall target ###############
#CONFIGURE_FILE(
#    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
#    IMMEDIATE @ONLY)

#ADD_CUSTOM_TARGET(uninstall
#    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")

########## Add make dist target ################
SET(CPACK_PACKAGE_VERSION "${SLART_VERSION}")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "slart-${CPACK_PACKAGE_VERSION}")
SET(CPACK_IGNORE_FILES "/CVS/;/\\\\.git\;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;\\\\.tar.gz$;/CMakeFiles/;CMakeCache.txt;refresh-copyright-and-license.pl")
SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
INCLUDE(CPack)
# simulate autotools' "make dist"
#add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

SET( TRANSLATIONS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/translations CACHE PATH "translations directory" )
SET( BIN_DIR ${CMAKE_BINARY_DIR}/bin CACHE PATH "binaries directory" )
SET( LIB_DIR ${CMAKE_BINARY_DIR}/lib CACHE PATH "libraries directory" )
SET( TEST_DIR ${CMAKE_BINARY_DIR}/test CACHE PATH "test binaries directory" )


#ADD_SUBDIRECTORY( doxygen )

ADD_SUBDIRECTORY( tools )
ADD_SUBDIRECTORY( libs )
ADD_SUBDIRECTORY( tests )
ADD_SUBDIRECTORY( apps )

