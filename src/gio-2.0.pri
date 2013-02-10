#############################################################################
# src/gio-2.0.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

PKGCONFIG += gio-2.0
# since PKGCONFIG alone does not seem to do the job, adding this:
INCLUDEPATH += $$system( pkg-config --cflags-only-I gio-2.0|sed 's/-I//g' )
QMAKE_CXXFLAGS += $$system( pkg-config --cflags-only-other gio-2.0 )
LIBS += $$system( pkg-config --libs gio-2.0 )
!build_pass:message("gio 2.0 configuration added. Check INCLUDEPATH and LIBS.")
