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

INCLUDEPATH += $${TOPSRC}/libs/Common $${TOPSRC}/libs/Magic
LIBS += -lCommon -lFLAC++
PKGCONFIG += vorbisenc libcdio_paranoia libcdio

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../../libs/Common/Resources/WinIcon.rc

SOURCES += Application.cpp

HEADERS += MainWidget.hpp
SOURCES += MainWidget.cpp

HEADERS += ConfigDialog.hpp
SOURCES += ConfigDialog.cpp

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

