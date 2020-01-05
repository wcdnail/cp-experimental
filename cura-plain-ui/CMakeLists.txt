cmake_minimum_required(VERSION 3.0)

project(cura-plain-ui)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
add_definitions(${GTK3_CFLAGS_OTHER})
include_directories(SYSTEM ${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

pkg_check_modules(JSONGLIB REQUIRED json-glib-1.0)
add_definitions(${JSONGLIB_CFLAGS_OTHER})
include_directories(SYSTEM ${JSONGLIB_INCLUDE_DIRS})
link_directories(${JSONGLIB_LIBRARY_DIRS})

file(GLOB_RECURSE CXX_SRCS *.cpp)
file(GLOB_RECURSE C_SRCS *.c)

add_executable(${PROJECT_NAME} "${C_SRCS} ${CXX_SRCS}")

target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
