cmake_minimum_required(VERSION 3.1.0)

project(ezV24)


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
add_library(ezV24_shared SHARED ${viscadump_SRCS})

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

set_property(TARGET ezV24_shared PROPERTY SOVERSION 1)
