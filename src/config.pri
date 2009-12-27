#############################################################################
# src/config.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

TOPLEVEL="../../.."

count( TARGET, 0 ) {
  error(TARGET not set $$TARGET)
}

build_pass:CONFIG(debug, debug|release) {
  TARGETARCH="debug"
} else {
  TARGETARCH="release"
}

contains(TEMPLATE,app) {
  is_test = $$find( OUT_PWD, "/tests/" ) 
  count( is_test, 1 ) {
    DESTDIR    = $${TOPLEVEL}/build/$${TARGETARCH}/test
  } else {
    DESTDIR    = $${TOPLEVEL}/build/$${TARGETARCH}/bin
  }
} else {
  DESTDIR      = $${TOPLEVEL}/build/$${TARGETARCH}/lib
}
LIBS           += -L$${TOPLEVEL}/build/$${TARGETARCH}/lib

DLLDESTDIR     = $${TOPLEVEL}/build/$${TARGETARCH}/lib
MOC_DIR        = $${TOPLEVEL}/build/$${TARGETARCH}/$${TARGET}/moc
OBJECTS_DIR    = $${TOPLEVEL}/build/$${TARGETARCH}/$${TARGET}/objs
RCC_DIR        = $${TOPLEVEL}/build/$${TARGETARCH}/$${TARGET}/rcc
UI_DIR         = $${TOPLEVEL}/build/$${TARGETARCH}/$${TARGET}/ui
UI_HEADERS_DIR = $${TOPLEVEL}/build/$${TARGETARCH}/$${TARGET}/ui_headers
UI_SOURCES_DIR = $${TOPLEVEL}/build/$${TARGETARCH}/$${TARGET}/ui_sources

contains( QMAKE_CXX, g++ ) {
  QMAKE_CXXFLAGS_DEBUG += -pedantic -Wno-long-long
  QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
  QMAKE_LFLAGS += -Wl,--gc-sections -rdynamic
}
