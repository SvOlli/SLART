#
# src/libs/CommandLine/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/CommandLine ====" )

PROJECT( CommandLine )

INCLUDE( LocalLibrary )

SET( CommandLine_PUBLIC_HEADERS
   CommandLine.hpp
)

SET( CommandLine_PRIVATE_HEADERS
   CommandLineOption.hpp
   CommandLineOptionBool.hpp
   CommandLineOptionString.hpp
)

SET( CommandLine_SOURCES
   CommandLine.cpp
   CommandLineOption.cpp
   CommandLineOptionBool.cpp
   CommandLineOptionString.cpp
)

ADD_LIBRARY( CommandLine STATIC
   ${CommandLine_PUBLIC_HEADERS}
   ${CommandLine_PRIVATE_HEADERS}
   ${CommandLine_SOURCES}
)

PROPAGATE_LIBRARY( CommandLine )
