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
INCLUDEPATH += $${TOPSRC}/libs/Satellite
LIBS += -lSatellite
QMAKE_CXXFLAGS_DEBUG += -DCOREDUMP_SIZE_MB=100

# Input
RESOURCES = $${TOPSRC}/resources/$${TARGET}.qrc
RC_FILE = $${TOPSRC}/resources/windows/$${TARGET}.rc

TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}.ts
TRANSLATIONS += $${TOPSRC}/resources/translations/$${TARGET}_de.ts

SOURCES += RubberbandmanApplication.cpp

HEADERS += BrowseWidget.hpp
SOURCES += BrowseWidget.cpp

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

HEADERS += MyTreeView.hpp
SOURCES += MyTreeView.cpp

HEADERS += RecurseWorker.hpp
SOURCES += RecurseWorker.cpp

HEADERS += RubberbandmanConfigDialog.hpp
SOURCES += RubberbandmanConfigDialog.cpp

HEADERS += RubberbandmanMainWidget.hpp
SOURCES += RubberbandmanMainWidget.cpp

HEADERS += SatelliteWidget.hpp
SOURCES += SatelliteWidget.cpp

HEADERS += TrackReader.hpp
SOURCES += TrackReader.cpp

HEADERS += TrackWriter.hpp
SOURCES += TrackWriter.cpp

