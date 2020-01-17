cmake_minimum_required(VERSION 3.0)

project(cp-experimental)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not specified.  Defaulting to 'Release'")
  message(STATUS "Usage: cmake -DCMAKE_BUILD_TYPE=[Debug|Release|RelWithDebInfo|MinSizeRel] .")
  set(CMAKE_BUILD_TYPE Release)
endif()

if (MSVC)
    set(VCPKG_ROOT_DIR "$ENV{VCPKG_ROOT}")
    if ("${VCPKG_ROOT_DIR}" STREQUAL "")
        message(FATAL_ERROR "VCPKG is not configured! (VCPKG_ROOT environment variable is empty)")
    endif()
    if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
        include(${CMAKE_TOOLCHAIN_FILE})
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(VCPKG_BUILDCONF_DIR "$ENV{VCPKG_ROOT}/installed/x64-windows/debug")
    else()
        set(VCPKG_BUILDCONF_DIR "$ENV{VCPKG_ROOT}/installed/x64-windows")
    endif()
endif()

set(REL_DEFS          "-DNDEBUG")
set(DBG_DEFS          "-D_DEBUG")
set(MSVC_CFLAGS       "/wd4047")
set(GNU_CFLAGS        "-Wno-incompatible-pointer-types \
-Wno-int-to-pointer-cast \
-Wno-pointer-to-int-cast \
")

set(MSVC_OPT_CFLAGS   "${CMAKE_C_FLAGS_RELEASE}")
set(GNU_OPT_CFLAGS    "-Os -Ofast ${REL_DEFS}")
set(OPT_CFLAGS        "${${CMAKE_C_COMPILER_ID}_OPT_CFLAGS}")

set(Debug_CFLAGS      "${CMAKE_C_FLAGS_DEBUG} ${DBG_DEFS}")
set(Release_CFLAGS    "${OPT_CFLAGS}")
set(MinSizeRel_CFLAGS "${OPT_CFLAGS}")

set(CONF_CFLAGS       "${${CMAKE_BUILD_TYPE}_CFLAGS}")
set(COMPILER_CFLAGS   "${${CMAKE_C_COMPILER_ID}_CFLAGS}")
set(CMAKE_C_FLAGS     "${ALL_DEFS} ${CONF_CFLAGS} ${COMPILER_CFLAGS}")
set(CMAKE_CXX_FLAGS   "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")

if (WIN32)
    add_definitions(-D_UNICODE)
    add_definitions(-DUNICODE)
    add_definitions(-D_WIN32)
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
endif()

#add_subdirectory(launch-pad-gtk)
add_subdirectory(launch-pad-qt)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT launch-pad-qt)
endif()
