#############################################################################
# src/buildconfig.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

contains(QT_VERSION, ^4\\.[0-3]\\..*) {
  message("Cannot build this software with Qt version $${QT_VERSION}.")
  error("Use at least Qt 4.4.")
}

count( TOPSRC, 0 ) {
  TOPSRC="../.."
}

count( TARGET, 0 ) {
  error(TARGET not set $$TARGET)
}

build_pass:CONFIG(release, debug|release) {
  TARGETARCH="release"
} else {
  TARGETARCH="debug"
}

contains(TEMPLATE,app) {
  is_test = $$find( OUT_PWD, "/tests/" ) 
  count( is_test, 1 ) {
    DESTDIR    = $${TOPSRC}/../build/$${TARGETARCH}/test
  } else {
    DESTDIR    = $${TOPSRC}/../build/$${TARGETARCH}/bin
  }
} else {
  contains( CONFIG, plugin ) {
    DESTDIR    = $${TOPSRC}/../build/$${TARGETARCH}/lib
  } else {
    DESTDIR    = $${TOPSRC}/../build/$${TARGETARCH}/tmp/lib
  }
}
LIBS           += -L$${TOPSRC}/../build/$${TARGETARCH}/tmp/lib

DLLDESTDIR     = $${TOPSRC}/../build/$${TARGETARCH}/lib
MOC_DIR        = $${TOPSRC}/../build/$${TARGETARCH}/tmp/$${TARGET}/moc
OBJECTS_DIR    = $${TOPSRC}/../build/$${TARGETARCH}/tmp/$${TARGET}/objs
RCC_DIR        = $${TOPSRC}/../build/$${TARGETARCH}/tmp/$${TARGET}/rcc
UI_DIR         = $${TOPSRC}/../build/$${TARGETARCH}/tmp/$${TARGET}/ui
UI_HEADERS_DIR = $${TOPSRC}/../build/$${TARGETARCH}/tmp/$${TARGET}/ui_headers
UI_SOURCES_DIR = $${TOPSRC}/../build/$${TARGETARCH}/tmp/$${TARGET}/ui_sources

contains( QMAKE_CXX, g++ ) {
  QMAKE_CXXFLAGS_DEBUG += -pedantic -Wno-long-long
  QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
  !macx {
    QMAKE_LFLAGS += -Wl,--gc-sections -rdynamic
  }
}
