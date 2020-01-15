cmake_minimum_required(VERSION 3.0)

project(cp-experimental)

# Default to Debug build
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not specified.  Defaulting to 'Release'")
  message(STATUS "Usage: cmake -DCMAKE_BUILD_TYPE=[Debug|Release|RelWithDebInfo|MinSizeRel] .")
  set(CMAKE_BUILD_TYPE Release)
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

add_subdirectory(launch-pad-qt)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT launch-pad-qt)
endif()
