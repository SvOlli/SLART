######################################################################
# src/apps/Stripped/Stripped.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Stripped
QT += network
CONFIG += link_pkgconfig

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/Magic
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += sql
}
PKGCONFIG += libcdio_paranoia libcdio
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/libs/Common/Resources/Common.qrc
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

SOURCES += StrippedApplication.cpp

HEADERS += CDReader.hpp
SOURCES += CDReader.cpp

HEADERS += CDReaderThread.hpp
SOURCES += CDReaderThread.cpp

HEADERS += CDEdit.hpp
SOURCES += CDEdit.cpp

HEADERS += CDEditCheckBox.hpp
SOURCES += CDEditCheckBox.cpp

HEADERS += CDEditLineEdit.hpp
SOURCES += CDEditLineEdit.cpp

HEADERS += CDInfo.hpp
SOURCES += CDInfo.cpp

HEADERS += CDDBClient.hpp
SOURCES += CDDBClient.cpp

HEADERS += ParanoiaStatus.hpp
SOURCES += ParanoiaStatus.cpp

HEADERS += StrippedConfigDialog.hpp
SOURCES += StrippedConfigDialog.cpp

HEADERS += StrippedMainWindow.hpp
SOURCES += StrippedMainWindow.cpp
