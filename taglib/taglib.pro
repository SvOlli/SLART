######################################################################
# hand hacked by SvOlli
######################################################################

include( ../Global.pri )

TEMPLATE = lib
CONFIG = staticlib
TARGET = tag
INCLUDEPATH += .
win32:INCLUDEPATH += /usr/local/include
VERSION = 1.5
DESTDIR =
QMAKE_CXXFLAGS = -DNDEBUG=NDEBUG

SOURCES = \
  apefooter.cpp \
  apeitem.cpp \
  apetag.cpp \
  attachedpictureframe.cpp \
  audioproperties.cpp \
  commentsframe.cpp \
  fileref.cpp \
  flacfile.cpp \
  flacproperties.cpp \
  generalencapsulatedobjectframe.cpp \
  id3v1genres.cpp \
  id3v1tag.cpp \
  id3v2extendedheader.cpp \
  id3v2footer.cpp \
  id3v2frame.cpp \
  id3v2framefactory.cpp \
  id3v2header.cpp \
  id3v2synchdata.cpp \
  id3v2tag.cpp \
  mpcfile.cpp \
  mpcproperties.cpp \
  mpegfile.cpp \
  mpegheader.cpp \
  mpegproperties.cpp \
  oggfile.cpp \
  oggflacfile.cpp \
  oggpage.cpp \
  oggpageheader.cpp \
  relativevolumeframe.cpp \
  speexfile.cpp \
  speexproperties.cpp \
  tag.cpp \
  tagunion.cpp \
  tbytevector.cpp \
  tbytevectorlist.cpp \
  tdebug.cpp \
  textidentificationframe.cpp \
  tfile.cpp \
  trueaudiofile.cpp \
  trueaudioproperties.cpp \
  tstring.cpp \
  tstringlist.cpp \
  unicode.cpp \
  uniquefileidentifierframe.cpp \
  unknownframe.cpp \
  unsynchronizedlyricsframe.cpp \
  urllinkframe.cpp \
  vorbisfile.cpp \
  vorbisproperties.cpp \
  wavpackfile.cpp \
  wavpackproperties.cpp \
  xingheader.cpp \
  xiphcomment.cpp \

HEADERS = \
  apefooter.h \
  apeitem.h \
  apetag.h \
  attachedpictureframe.h \
  audioproperties.h \
  commentsframe.h \
  fileref.h \
  flacfile.h \
  flacproperties.h \
  generalencapsulatedobjectframe.h \
  id3v1genres.h \
  id3v1tag.h \
  id3v2extendedheader.h \
  id3v2footer.h \
  id3v2framefactory.h \
  id3v2frame.h \
  id3v2header.h \
  id3v2synchdata.h \
  id3v2tag.h \
  mpcfile.h \
  mpcproperties.h \
  mpegfile.h \
  mpegheader.h \
  mpegproperties.h \
  oggfile.h \
  oggflacfile.h \
  oggpage.h \
  oggpageheader.h \
  relativevolumeframe.h \
  speexfile.h \
  speexproperties.h \
  tag.h \
  taglib_export.h \
  taglib.h \
  tagunion.h \
  tbytevector.h \
  tbytevectorlist.h \
  tdebug.h \
  textidentificationframe.h \
  tfile.h \
  tlist.h \
  tmap.h \
  trueaudiofile.h \
  trueaudioproperties.h \
  tstring.h \
  tstringlist.h \
  unicode.h \
  uniquefileidentifierframe.h \
  unknownframe.h \
  unsynchronizedlyricsframe.h \
  urllinkframe.h \
  vorbisfile.h \
  vorbisproperties.h \
  wavpackfile.h \
  wavpackproperties.h \
  xingheader.h \
  xiphcomment.h \
  tlist.tcc \
  tmap.tcc \

