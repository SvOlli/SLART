######################################################################
# src/apps/Funkytown/Funkytown.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Funkytown
TEMPLATE = app
QT += network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
contains( TARGETARCH, debug ) {
QT += sql
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}_de.ts

SOURCES += FunkytownApplication.cpp

HEADERS += Cookie.hpp
SOURCES += Cookie.cpp

HEADERS += CookieJar.hpp
SOURCES += CookieJar.cpp

HEADERS += DownloadHandler.hpp
SOURCES += DownloadHandler.cpp

HEADERS += FunkytownConfigDialog.hpp
SOURCES += FunkytownConfigDialog.cpp

HEADERS += FunkytownMainWidget.hpp
SOURCES += FunkytownMainWidget.cpp

HEADERS += MagicQueue.hpp
SOURCES += MagicQueue.cpp

HEADERS += TheMagic.hpp
SOURCES += TheMagic.cpp
