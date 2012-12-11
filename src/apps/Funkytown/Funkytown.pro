######################################################################
# src/apps/Funkytown/Funkytown.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TARGET = Funkytown
TEMPLATE = app
QT += network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/libs/Common/Resources/Common.qrc
RESOURCES += Resources/Funkytown.qrc
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

SOURCES += Application.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += Cookie.hpp
SOURCES += Cookie.cpp

HEADERS += CookieJar.hpp
SOURCES += CookieJar.cpp

HEADERS += DownloadHandler.hpp
SOURCES += DownloadHandler.cpp

HEADERS += FunkytownMainWidget.hpp
SOURCES += FunkytownMainWidget.cpp

HEADERS += MagicQueue.hpp
SOURCES += MagicQueue.cpp

HEADERS += TheMagic.hpp
SOURCES += TheMagic.cpp
