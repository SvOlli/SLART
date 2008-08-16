######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = app
TARGET = Stripped

#LIBS += -lvorbisenc -lvorbis -lm -logg -lcdio_paranoia -lcdio_cdda -lcdio
QMAKE_CXXFLAGS += $$system( sh ../configure --cflags vorbisenc )
LIBS += $$system( sh ../configure --libs vorbisenc libcdio_paranoia )
# Input
#RESOURCES = Resources.qrc

SOURCES += Application.cpp 

HEADERS += MainWindow.hpp 
SOURCES += MainWindow.cpp 

HEADERS += MainWidget.hpp 
SOURCES += MainWidget.cpp 

HEADERS += ConfigDialog.hpp 
SOURCES += ConfigDialog.cpp 

HEADERS += GlobalConfigWidget.hpp 
SOURCES += GlobalConfigWidget.cpp 

HEADERS += MySettings.hpp 
SOURCES += MySettings.cpp 

HEADERS += AboutWidget.hpp
SOURCES += AboutWidget.cpp

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

HEADERS += RawEncoder.hpp 
SOURCES += RawEncoder.cpp 

HEADERS += OggEncoder.hpp 
SOURCES += OggEncoder.cpp 

HEADERS += TagList.hpp 
SOURCES += TagList.cpp 

