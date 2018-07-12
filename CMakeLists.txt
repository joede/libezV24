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


## Include directories usage requirements commonly differ between the build-tree
## and the install-tree. The BUILD_INTERFACE and INSTALL_INTERFACE generator
## expressions can be used to describe separate usage requirements based on the
## usage location. Relative paths are allowed within the INSTALL_INTERFACE
## expression and are interpreted relative to the installation prefix.
##
## If we only use one single relative path, Cmake will add the prefix to have a
## absulote path at the time the compiler is called.
##
target_include_directories(ezV24_static
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ezV24>
  $<INSTALL_INTERFACE:include/ezV24>  # <prefix>/include/ezV24
PRIVATE src
)
target_include_directories(ezV24_shared
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ezV24>
  $<INSTALL_INTERFACE:include/ezV24>  # <prefix>/include/ezV24
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
## Installation of the general library stuff
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


## -------------------------------------------------------------------
## Generation and Installation of the CMake support stuff
##
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ezV24/ezV24ConfigVersion.cmake
  VERSION ${ezV24_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT ezV24_Targets
  FILE ezV24/ezV24Targets.cmake
  NAMESPACE ezV24::
)

set(ConfigPackageLocation lib/cmake/ezV24)

#~ configure_file(cmake/ezV24-config.cmake.in
  #~ ezV24/ezV24Config.cmake
  #~ @ONLY
#~ )
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ezV24-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ezV24/ezV24Config.cmake
    INSTALL_DESTINATION ${ConfigPackageLocation}
)


install(EXPORT ezV24_Targets
  FILE
    ezV24Targets.cmake
#    ${CMAKE_CURRENT_BINARY_DIR}/ezV24/ezV24Targets.cmake
  NAMESPACE
    ezV24::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ezV24/ezV24Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ezV24/ezV24ConfigVersion.cmake
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)
