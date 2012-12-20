#
# src/libs/TagLib/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/TagLib ====" )

PROJECT( TagLib )

INCLUDE( LocalTools )

INCLUDE_DIRECTORIES(
   .
)

SET( TagLib_PRIVATE_HEADERS
   tlist.tcc
   tmap.tcc
)

ADD_SOURCE( apefooter                      cpp h )
ADD_SOURCE( apeitem                        cpp h )
ADD_SOURCE( apetag                         cpp h )
ADD_SOURCE( attachedpictureframe           cpp h )
ADD_SOURCE( audioproperties                cpp h )
ADD_SOURCE( commentsframe                  cpp h )
ADD_SOURCE( fileref                        cpp h )
ADD_SOURCE( flacfile                       cpp h )
ADD_SOURCE( flacproperties                 cpp h )
ADD_SOURCE( generalencapsulatedobjectframe cpp h )
ADD_SOURCE( id3v1genres                    cpp h )
ADD_SOURCE( id3v1tag                       cpp h )
ADD_SOURCE( id3v2extendedheader            cpp h )
ADD_SOURCE( id3v2footer                    cpp h )
ADD_SOURCE( id3v2frame                     cpp h )
ADD_SOURCE( id3v2framefactory              cpp h )
ADD_SOURCE( id3v2header                    cpp h )
ADD_SOURCE( id3v2synchdata                 cpp h )
ADD_SOURCE( id3v2tag                       cpp h )
ADD_SOURCE( mpcfile                        cpp h )
ADD_SOURCE( mpcproperties                  cpp h )
ADD_SOURCE( mpegfile                       cpp h )
ADD_SOURCE( mpegheader                     cpp h )
ADD_SOURCE( mpegproperties                 cpp h )
ADD_SOURCE( oggfile                        cpp h )
ADD_SOURCE( oggflacfile                    cpp h )
ADD_SOURCE( oggpage                        cpp h )
ADD_SOURCE( oggpageheader                  cpp h )
ADD_SOURCE( relativevolumeframe            cpp h )
ADD_SOURCE( speexfile                      cpp h )
ADD_SOURCE( speexproperties                cpp h )
ADD_SOURCE( tag                            cpp h )
ADD_SOURCE( taglib_export                      h )
ADD_SOURCE( taglib                             h )
ADD_SOURCE( tagunion                       cpp h )
ADD_SOURCE( tbytevector                    cpp h )
ADD_SOURCE( tbytevectorlist                cpp h )
ADD_SOURCE( tdebug                         cpp h )
ADD_SOURCE( textidentificationframe        cpp h )
ADD_SOURCE( tfile                          cpp h )
ADD_SOURCE( tlist                              h )
ADD_SOURCE( tmap                               h )
ADD_SOURCE( trueaudiofile                  cpp h )
ADD_SOURCE( trueaudioproperties            cpp h )
ADD_SOURCE( tstring                        cpp h )
ADD_SOURCE( tstringlist                    cpp h )
ADD_SOURCE( unicode                        cpp h )
ADD_SOURCE( uniquefileidentifierframe      cpp h )
ADD_SOURCE( unknownframe                   cpp h )
ADD_SOURCE( unsynchronizedlyricsframe      cpp h )
ADD_SOURCE( urllinkframe                   cpp h )
ADD_SOURCE( vorbisfile                     cpp h )
ADD_SOURCE( vorbisproperties               cpp h )
ADD_SOURCE( wavpackfile                    cpp h )
ADD_SOURCE( wavpackproperties              cpp h )
ADD_SOURCE( xingheader                     cpp h )
ADD_SOURCE( xiphcomment                    cpp h )

ADD_LIBRARY( TagLib STATIC
   ${TagLib_HEADERS}
   ${TagLib_PRIVATE_HEADERS}
   ${TagLib_SOURCES}
)

PROPAGATE_LIBRARY( TagLib )
