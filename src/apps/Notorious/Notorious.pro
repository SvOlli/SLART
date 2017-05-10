######################################################################
# src/apps/Notorious/Notorious.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Notorious
CONFIG += qt
QT += widgets sql
LIBS += -lbz2

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon
contains( TARGETARCH, debug ) {
QT += widgets network
}
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}_de.ts

SOURCES += NotoriousApplication.cpp

HEADERS += Foundlist.hpp
SOURCES += Foundlist.cpp

HEADERS += FreeDB.hpp
SOURCES += FreeDB.cpp

HEADERS += FreeDBEntry.hpp
SOURCES += FreeDBEntry.cpp

HEADERS += FreeDBImport.hpp
SOURCES += FreeDBImport.cpp

HEADERS += FreeDBQuery.hpp
SOURCES += FreeDBQuery.cpp

HEADERS += NotoriousConfigDialog.hpp
SOURCES += NotoriousConfigDialog.cpp

HEADERS += NotoriousMainWidget.hpp
SOURCES += NotoriousMainWidget.cpp

HEADERS += SearchWidget.hpp
SOURCES += SearchWidget.cpp

HEADERS += TarEntry.hpp
SOURCES += TarEntry.cpp

HEADERS += tir.h
SOURCES += tir.c
