#
# src/tests/TestAppIconView/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/tests/TextAppIconView ====" )

PROJECT( TextAppIconView )

INCLUDE( LocalTools )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

ADD_SOURCE( Application cpp         )
ADD_SOURCE( MainWidget  cpp hpp moc )
ADD_SOURCE( MainWindow  cpp hpp moc )

QT4_WRAP_CPP( TextAppIconView_MOC_GENERATED
   ${TextAppIconView_MOC_HEADERS}
)

ADD_EXECUTABLE( TextAppIconView
   ${TextAppIconView_MOC_GENERATED}
   ${TextAppIconView_HEADERS}
   ${TextAppIconView_SOURCES}
)

TARGET_LINK_LIBRARIES( TextAppIconView
   ${QT_LIBRARIES}
)

IF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
   INSTALL( TARGETS TextAppIconView
      DESTINATION test
   )
ENDIF( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg] )
