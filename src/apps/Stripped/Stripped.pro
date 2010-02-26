######################################################################
# src/apps/Stripped/Stripped.pro
# written by Sven Oliver Moll
# distributed as public domain
######################################################################

TEMPLATE = app
TARGET = Stripped
QT += sql network
CONFIG += link_pkgconfig

include( ../../config.pri )

INCLUDEPATH += ../../libs/Common
LIBS += -lCommon -lFLAC++
PKGCONFIG += vorbisenc libcdio_paranoia libcdio

# Input
RESOURCES = ../../libs/Common/Resources/Common.qrc
RC_FILE = ../../libs/Common/Resources/WinIcon.rc

SOURCES += Application.cpp 

#HEADERS += Database.hpp

#HEADERS += MainWindow.hpp 
#SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

#HEADERS += GlobalConfigWidget.hpp 
#SOURCES += GlobalConfigWidget.cpp 

#HEADERS += ProxyWidget.hpp 
#SOURCES += ProxyWidget.cpp 

#HEADERS += MySettings.hpp 
#SOURCES += MySettings.cpp 

#HEADERS += AboutWidget.hpp
#SOURCES += AboutWidget.cpp

HEADERS += CDReader.hpp 
SOURCES += CDReader.cpp 

HEADERS += CDEdit.hpp 
SOURCES += CDEdit.cpp 

HEADERS += CDToc.hpp 
SOURCES += CDToc.cpp 

HEADERS += CDDB.hpp 
SOURCES += CDDB.cpp 

HEADERS += Encoder.hpp 
SOURCES += Encoder.cpp 

HEADERS += WavEncoder.hpp 
SOURCES += WavEncoder.cpp 

HEADERS += OggEncoder.hpp 
SOURCES += OggEncoder.cpp 

HEADERS += FlacEncoder.hpp 
SOURCES += FlacEncoder.cpp 

#HEADERS += TagList.hpp 
#SOURCES += TagList.cpp 

