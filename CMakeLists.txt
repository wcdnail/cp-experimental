cmake_minimum_required(VERSION 3.0)

project(cura-plain)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()

add_subdirectory(cura-plain-ui)
