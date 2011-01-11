######################################################################
# src/apps/Karmadrome/Karmadrome.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Karmadrome
QT += sql network

include( ../../buildconfig.pri )

INCLUDEPATH += $${TOPSRC}/libs/Common
LIBS += -lCommon

# Input
RESOURCES = $${TOPSRC}/libs/Common/Resources/Common.qrc
RC_FILE = $${TOPSRC}/libs/Common/Resources/WinIcon.rc

TRANSLATIONS += $${TOPSRC}/translations/$${TARGET}_de.ts

SOURCES += Application.cpp

HEADERS += ButtonsWidget.hpp
SOURCES += ButtonsWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

HEADERS += ExportFolder.hpp
SOURCES += ExportFolder.cpp

HEADERS += ExportFolderList.hpp
SOURCES += ExportFolderList.cpp

HEADERS += ImportFolder.hpp
SOURCES += ImportFolder.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

