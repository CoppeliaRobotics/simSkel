cmake_minimum_required(VERSION 2.8.11)
project(v_repExtPluginSkeletonNG)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost REQUIRED)

# uncomment following if plugin uses Qt:
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5PrintSupport REQUIRED)
#if(WIN32)
#	set(SHLWAPI shlwapi)
#else()
#	set(SHLWAPI)
#endif()


if(NOT DEFINED ENV{VREP_ROOT})
	message(FATAL_ERROR "Environment variable VREP_ROOT is not set")
endif()
file(TO_CMAKE_PATH "$ENV{VREP_ROOT}" VREP_ROOT)

include_directories(${VREP_ROOT}/programming/include)
include_directories(${CMAKE_BINARY_DIR}/generated)
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${Boost_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/generated/stubs.cpp ${CMAKE_BINARY_DIR}/generated/stubs.h ${CMAKE_BINARY_DIR}/generated/lua_calltips.cpp
    COMMAND python ${CMAKE_SOURCE_DIR}/external/v_repStubsGen/generate.py --xml-file ${CMAKE_SOURCE_DIR}/callbacks.xml --lua-file ${CMAKE_SOURCE_DIR}/simExtPluginSkeletonNG.lua --gen-all ${CMAKE_BINARY_DIR}/generated
    DEPENDS ${CMAKE_SOURCE_DIR}/callbacks.xml)

set(SOURCES
    v_repExtPluginSkeletonNG.cpp
    external/v_repPlusPlus/Plugin.cpp
)
add_library(v_repExtPluginSkeletonNG SHARED ${SOURCES} ${CMAKE_BINARY_DIR}/generated/stubs.cpp "${VREP_ROOT}/programming/common/v_repLib.cpp")

target_link_libraries(v_repExtPluginSkeletonNG ${Boost_LIBRARIES})
#target_link_libraries(v_repExtPluginSkeletonNG ${Boost_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport ${SHLWAPI})
