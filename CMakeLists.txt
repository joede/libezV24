cmake_minimum_required(VERSION 3.1.0)

project(ezV24)
set(ezV24_VERSION 0.1.4)
set(ezV24_SOVERSION 1)


## Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(viscadump_SRCS
  include/ezV24/ezV24.h
  src/ezV24.c
  src/ezV24_config.h
  src/snprintf.c
  src/snprintf.h
)

add_library(ezV24_static STATIC ${viscadump_SRCS})
set_property(TARGET ezV24_static PROPERTY VERSION ${ezV24_VERSION})
add_library(ezV24_shared SHARED ${viscadump_SRCS})
set_property(TARGET ezV24_shared PROPERTY VERSION ${ezV24_VERSION})
## what is this good for? Only needed for shared libraries?
## ste this to 42 to check where it goes...
set_property(TARGET ezV24_shared PROPERTY
  INTERFACE_ezV24_shared_MAJOR_VERSION 42)
set_property(TARGET ezV24_shared APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING ezV24_shared_MAJOR_VERSION
)


target_include_directories(ezV24_static
PUBLIC include/ezV24
PRIVATE src
)
target_include_directories(ezV24_shared
PUBLIC include/ezV24
PRIVATE src
)

if(WIN32)
  #set_target_properties(ezV24 PROPERTIES COMPILE_DEFINITIONS "__WIN32__=1")
  target_compile_definitions(ezV24_static PRIVATE __WIN32__=1)
  target_compile_definitions(ezV24_shared PRIVATE __WIN32__=1)
elseif(APPLE)
  #set_target_properties(ezV24 PROPERTIES COMPILE_DEFINITIONS "__APPLE__=1")
  target_compile_definitions(ezV24_static PRIVATE __APPLE__=1)
  target_compile_definitions(ezV24_shared PRIVATE __APPLE__=1)
else()
  #set_target_properties(ezV24 PROPERTIES COMPILE_DEFINITIONS "__LINUX__=1")
  target_compile_definitions(ezV24_static PRIVATE __LINUX__=1)
  target_compile_definitions(ezV24_shared PRIVATE __LINUX__=1)
endif()

set_property(TARGET ezV24_shared PROPERTY SOVERSION ${ezV24_SOVERSION})

## -------------------------------------------------------------------
##
install(TARGETS ezV24_static ezV24_shared EXPORT ezV24_Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include/ezV24
)
install(
  FILES
    include/ezV24/ezV24.h
  DESTINATION
    include/ezV24/
  COMPONENT
    Devel
)
