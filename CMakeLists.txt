cmake_minimum_required(VERSION 2.8.11)
project(simExtPluginSkeletonNG)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost REQUIRED regex)

# uncomment following if plugin uses Qt:
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5PrintSupport REQUIRED)
#set(CMAKE_AUTOMOC ON)
#add_definitions(-DQT_COMPIL)

if(NOT LIBPLUGIN_DIR)
    if(DEFINED ENV{COPPELIASIM_ROOT_DIR})
        set(LIBPLUGIN_DIR $ENV{COPPELIASIM_ROOT_DIR}/programming/libPlugin)
    else()
        set(LIBPLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libPlugin)
    endif()
endif()
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
    ${LIBPLUGIN_DIR}/cmake)
find_package(CoppeliaSim 4.1.0.0 REQUIRED)

include_directories(${COPPELIASIM_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${Boost_INCLUDE_DIRS})

coppeliasim_generate_stubs(${CMAKE_CURRENT_BINARY_DIR}/generated XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/callbacks.xml LUA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/simExtPluginSkeletonNG.lua)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h ESCAPE_QUOTES)

set(SOURCES
    plugin.cpp
    ${COPPELIASIM_EXPORTED_SOURCES}
)
add_library(simExtPluginSkeletonNG SHARED ${SOURCES})
target_link_libraries(simExtPluginSkeletonNG ${Boost_LIBRARIES} ${COPPELIASIM_LIBRARIES})
#target_link_libraries(simExtPluginSkeletonNG ${Boost_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport ${COPPELIASIM_LIBRARIES})

install(TARGETS simExtPluginSkeletonNG DESTINATION ${COPPELIASIM_PLUGINS_DIR})

