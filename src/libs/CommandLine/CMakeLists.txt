#
# src/libs/CommandLine/CMakeLists.txt
#
# written by SvOlli
# distributed as public domain
#

MESSAGE( STATUS "==== src/libs/CommandLine ====" )

PROJECT( CommandLine )

INCLUDE( LocalTools )

ADD_SOURCE( CommandLine             cpp hpp         )
ADD_SOURCE( CommandLineOption       cpp hpp private )
ADD_SOURCE( CommandLineOptionBool   cpp hpp private )
ADD_SOURCE( CommandLineOptionString cpp hpp private )

ADD_LIBRARY( CommandLine STATIC
   ${CommandLine_HEADERS}
   ${CommandLine_SOURCES}
)

PROPAGATE_LIBRARY( CommandLine )
