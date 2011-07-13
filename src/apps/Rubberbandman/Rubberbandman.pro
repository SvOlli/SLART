######################################################################
# src/apps/Rubberbandman/Rubberbandman.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Rubberbandman
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/TagLib
LIBS += -lCommon -lTag
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/libs/Common/Resources/Common.qrc
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}.ts
TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

SOURCES += Application.cpp

HEADERS += BrowseWidget.hpp
SOURCES += BrowseWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += Console.hpp
SOURCES += Console.cpp

HEADERS += DatabaseWidget.hpp
SOURCES += DatabaseWidget.cpp

HEADERS += DatabaseWorker.hpp
SOURCES += DatabaseWorker.cpp

HEADERS += DirWalkerDelete.hpp
SOURCES += DirWalkerDelete.cpp

HEADERS += DirWalkerMove.hpp
SOURCES += DirWalkerMove.cpp

HEADERS += DirWalkerUpdate.hpp
SOURCES += DirWalkerUpdate.cpp

HEADERS += FileSysBrowser.hpp
SOURCES += FileSysBrowser.cpp

HEADERS += InfoEdit.hpp
SOURCES += InfoEdit.cpp

HEADERS += LineEdit.hpp
SOURCES += LineEdit.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += MyTreeView.hpp
SOURCES += MyTreeView.cpp

HEADERS += RecurseWorker.hpp
SOURCES += RecurseWorker.cpp

HEADERS += SatelliteWidget.hpp
SOURCES += SatelliteWidget.cpp

HEADERS += TrackReader.hpp
SOURCES += TrackReader.cpp

HEADERS += TrackWriter.hpp
SOURCES += TrackWriter.cpp

