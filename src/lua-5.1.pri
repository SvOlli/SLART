#############################################################################
# src/lua-5.1.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

PKGCONFIG += lua5.1
# since PKGCONFIG alone does not seem to do the job, adding this:
INCLUDEPATH += $$system( pkg-config --cflags-only-I lua5.1|sed 's/-I//g' )
QMAKE_CXXFLAGS += $$system( pkg-config --cflags-only-other lua5.1 )
LIBS += $$system( pkg-config --libs lua5.1 )
!build_pass:message("Lua 5.1 configuration added. Check INCLUDEPATH and LIBS.")
