#
# src/tests/TestAppDragAndDrop/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppDragAndDrop ====" )

PROJECT( TestAppDragAndDrop )

#SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
INCLUDE_DIRECTORIES(
   ${COMMON_INCLUDE_DIR}
)

SET( TestAppDragAndDrop_HEADERS
   MainWidget.hpp
)

SET( TestAppDragAndDrop_SOURCES
   Application.cpp
   MainWidget.cpp
)

QT4_WRAP_CPP( TestAppDragAndDrop_MOC_HEADERS
   ${TestAppDragAndDrop_HEADERS}
)

ADD_EXECUTABLE( TestAppDragAndDrop
   ${TestAppDragAndDrop_MOC_HEADERS}
   ${TestAppDragAndDrop_HEADERS}
   ${TestAppDragAndDrop_SOURCES}
)

ADD_DEPENDENCIES( TestAppDragAndDrop
   Common
)

TARGET_LINK_LIBRARIES( TestAppDragAndDrop
   ${COMMON_LIBRARY_FLAGS}
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TestAppDragAndDrop
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
