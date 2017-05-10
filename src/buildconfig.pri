#############################################################################
# src/buildconfig.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

contains(QT_VERSION, ^5\\.[0-2]\\..*) {
  message("Cannot build this software with Qt version $${QT_VERSION}.")
  error("Use at least Qt 5.3.")
}

count( TOPSRC, 0 ) {
  TOPSRC = "../.."
}
count( BUILDDIR, 0 ) {
  BUILDDIR = $${OUT_PWD}/$${TOPSRC}
  message("BUILDDIR=$${BUILDDIR}")
}
#BUILDDIR = $${TOPSRC}/../build

count( TARGET, 0 ) {
  error(TARGET not set $$TARGET)
}

contains( TEMPLATE, app ) {
  is_tool = $$find( OUT_PWD, "/tools/" )
  is_test = $$find( OUT_PWD, "/tests/" )
  count( is_tool, 1 ) {
    DESTDIR    = $${BUILDDIR}/tools
  } else {
    count( is_test, 1 ) {
      DESTDIR  = $${BUILDDIR}/test
    } else {
      DESTDIR  = $${BUILDDIR}/bin
    }
  }
} else {
  contains( CONFIG, plugin ) {
    DESTDIR    = $${BUILDDIR}/lib
  } else {
    DESTDIR    = $${BUILDDIR}/tmp/lib
  }
}
LIBS           += -L$${BUILDDIR}/tmp/lib
message( "LIBS=$${LIBS}" )

DLLDESTDIR     = $${BUILDDIR}/lib
#MOC_DIR        = $${BUILDDIR}/tmp/$${TARGET}/moc
#OBJECTS_DIR    = $${BUILDDIR}/tmp/$${TARGET}/objs
#RCC_DIR        = $${BUILDDIR}/tmp/$${TARGET}/rcc
#UI_DIR         = $${BUILDDIR}/tmp/$${TARGET}/ui
#UI_HEADERS_DIR = $${BUILDDIR}/tmp/$${TARGET}/ui_headers
#UI_SOURCES_DIR = $${BUILDDIR}/tmp/$${TARGET}/ui_sources

CODECFORSRC    = UTF-8
CODECFORTR     = UTF-8
#QMAKE_CXXFLAGS += -DQT_NO_CAST_FROM_ASCII=QT_NO_CAST_FROM_ASCII
QMAKE_CXXFLAGS += -DQT_NO_CAST_TO_ASCII=QT_NO_CAST_TO_ASCII
QMAKE_CXXFLAGS += -DQT_NO_CAST_FROM_BYTEARRAY=QT_NO_CAST_FROM_BYTEARRAY
QMAKE_CXXFLAGS += -DQT_NO_URL_CAST_FROM_STRING=QT_NO_URL_CAST_FROM_STRING
#QMAKE_CXXFLAGS += -DQT_NO_KEYWORDS=QT_NO_KEYWORDS

contains( QMAKE_CXX, g++ ) {
  QMAKE_CXXFLAGS += -fstack-protector-strong -fPIC
  QMAKE_CXXFLAGS_DEBUG += -pedantic -Wno-long-long -DDEBUG=DEBUG
  QMAKE_CXXFLAGS_RELEASE += -ffunction-sections -fdata-sections
  #QMAKE_CXXFLAGS_DEBUG += -ffunction-sections -fdata-sections
  !macx {
    QMAKE_LFLAGS_RELEASE += -Wl,--gc-sections -rdynamic
    QMAKE_LFLAGS_DEBUG += -rdynamic
    #QMAKE_LFLAGS_DEBUG += -Wl,--gc-sections -rdynamic
  }
}
