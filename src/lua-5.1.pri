#############################################################################
# src/lua-5.1.pri
# hand hacked by SvOlli
# released as public domain
#############################################################################

PKGCONFIG += lua5.1
# since PKGCONFIG alone does not seem to do the job, adding this:
INCLUDEPATH += $$system( pkg-config --cflags-only-I lua5.1|sed 's/-I//g' )
QMAKE_CXXFLAGS += $$system( pkg-config --cflags-only-other lua5.1 )
LUA_LIBS = $$system( pkg-config --libs lua5.1 )
LIBS += $$LUA_LIBS
count( LUA_LIBS, 0 ) {
   error( "Lua 5.1 was not found" )
} else {
   !build_pass:message( "Lua 5.1 configuration added." )
}
