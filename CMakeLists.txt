cmake_minimum_required(VERSION 3.0)

project(cp-experimental)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Usage: cmake -DCMAKE_BUILD_TYPE=[Debug|Release|RelWithDebInfo|MinSizeRel]")
  message(FATAL_ERROR "CMAKE_BUILD_TYPE not specified")
endif()

set(VCPKG_ROOT_DIR "$ENV{VCPKG_ROOT}")
if ("${VCPKG_ROOT_DIR}" STREQUAL "")
    message(FATAL_ERROR "VCPKG is not configured! (VCPKG_ROOT environment variable is empty)")
endif()
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
    include(${CMAKE_TOOLCHAIN_FILE})
endif()

set(VCPKG_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VCPKG_BUILDCONF_DIR "${VCPKG_PREFIX_PATH}/debug")
else()
    set(VCPKG_BUILDCONF_DIR "${VCPKG_PREFIX_PATH}")
endif()

if (WIN32)
    add_definitions(-D_UNICODE
        -DUNICODE
        -D_WIN32
    )
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

#add_subdirectory(launch-pad-gtk)
#add_subdirectory(launch-pad-qt)
add_subdirectory(twins2)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT launch-pad-qt)
endif()

