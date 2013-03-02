#############################################################################
# src/gio-2.0.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

PKGCONFIG += gio-2.0
# since PKGCONFIG alone does not seem to do the job, adding this:
INCLUDEPATH += $$system( pkg-config --cflags-only-I gio-2.0|sed 's/-I//g' )
QMAKE_CXXFLAGS += $$system( pkg-config --cflags-only-other gio-2.0 )
GIO_LIBS = $$system( pkg-config --libs gio-2.0 )
LIBS += $$GIO_LIBS
count( GIO_LIBS, 0 ) {
   error( "gio 2.0 was not found" )
} else {
   !build_pass:message( "gio 2.0 configuration added." )
}
