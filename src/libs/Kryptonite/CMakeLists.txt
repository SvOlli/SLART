#
# src/libs/Kryptonite/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/Kryptonite ====" )

PROJECT( Kryptonite )

INCLUDE( LocalTools )

SET( QT_USE_QTNETWORK 1 )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ADD_DEFINITIONS( -fPIC )

ADD_SOURCE( IODeviceStreamer              cpp hpp moc )
ADD_SOURCE( Kryptonite                    cpp hpp moc )
ADD_SOURCE( KryptoniteJobCover            cpp hpp moc )
ADD_SOURCE( KryptoniteJobCoverAmazonDE    cpp hpp moc )
ADD_SOURCE( KryptoniteJobCoverDiscogs     cpp hpp moc )
ADD_SOURCE( KryptoniteReply               cpp hpp moc )

QT4_WRAP_CPP( Kryptonite_MOC_GENERATED
   ${Kryptonite_MOC_HEADERS}
)

ADD_LIBRARY( Kryptonite STATIC
   ${Kryptonite_MOC_GENERATED}
   ${Kryptonite_HEADERS}
   ${Kryptonite_SOURCES}
)

PROPAGATE_LIBRARY( Kryptonite )
