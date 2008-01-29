######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = lib
CONFIG = staticlib
TARGET = tag
INCLUDEPATH += .
win32:INCLUDEPATH += /usr/local/include
VERSION = 1.4
DESTDIR =

SOURCES = \
   apetag.cpp \
   apeitem.cpp \
   apefooter.cpp \
   audioproperties.cpp \
   fileref.cpp \
   flacproperties.cpp \
   flacfile.cpp \
   mpcfile.cpp \
   mpcproperties.cpp \
   id3v1tag.cpp \
   id3v1genres.cpp \
   id3v2footer.cpp \
   uniquefileidentifierframe.cpp \
   relativevolumeframe.cpp \
   attachedpictureframe.cpp \
   commentsframe.cpp \
   textidentificationframe.cpp \
   unknownframe.cpp \
   id3v2header.cpp \
   id3v2tag.cpp \
   id3v2synchdata.cpp \
   id3v2framefactory.cpp \
   id3v2frame.cpp \
   id3v2extendedheader.cpp \
   mpegproperties.cpp \
   xingheader.cpp \
   mpegfile.cpp \
   mpegheader.cpp \
   oggflacfile.cpp \
   oggpageheader.cpp \
   oggpage.cpp \
   xiphcomment.cpp \
   oggfile.cpp \
   vorbisfile.cpp \
   vorbisproperties.cpp \
   tag.cpp \
   tbytevectorlist.cpp \
   tstringlist.cpp \
   tdebug.cpp \
   unicode.cpp \
   tbytevector.cpp \
   tfile.cpp \
   tstring.cpp \

HEADERS = \
   apetag.h \
   apefooter.h \
   apeitem.h \
   audioproperties.h \
   fileref.h \
   flactag.h \
   flacfile.h \
   flacproperties.h \
   mpcfile.h \
   mpcproperties.h \
   combinedtag.h \
   xingheader.h \
   id3v1genres.h \
   id3v1tag.h \
   id3v2framefactory.h \
   id3v2header.h \
   uniquefileidentifierframe.h \
   attachedpictureframe.h \
   unknownframe.h \
   relativevolumeframe.h \
   textidentificationframe.h \
   commentsframe.h \
   id3v2footer.h \
   id3v2tag.h \
   id3v2synchdata.h \
   id3v2frame.h \
   id3v2extendedheader.h \
   mpegheader.h \
   mpegfile.h \
   mpegproperties.h \
   oggflacfile.h \
   oggfile.h \
   oggpage.h \
   vorbisfile.h \
   vorbisproperties.h \
   xiphcomment.h \
   oggpageheader.h \
   tag.h \
   tdebug.h \
   unicode.h \
   tfile.h \
   tlist.h \
   tbytevectorlist.h \
   tstring.h \
   taglib.h \
   tmap.h \
   tstringlist.h \
   tbytevector.h \
   tlist.tcc \
   tmap.tcc \

