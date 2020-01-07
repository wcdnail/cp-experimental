cmake_minimum_required(VERSION 3.0)

project(launch-pad)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
add_definitions(${GTK3_CFLAGS_OTHER})
include_directories(SYSTEM ${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

pkg_check_modules(JSONGLIB REQUIRED json-glib-1.0)
add_definitions(${JSONGLIB_CFLAGS_OTHER})
include_directories(SYSTEM ${JSONGLIB_INCLUDE_DIRS})
link_directories(${JSONGLIB_LIBRARY_DIRS})

set(RESOURCES_CT "${PROJECT_NAME}-resources")
set(RESOURCES_PN "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCES_CT}.c")

add_custom_command(
    OUTPUT "${RESOURCES_PN}"
    COMMAND glib-compile-resources
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.gresource.xml 
         --target=${RESOURCES_PN} 
         --sourcedir=${CMAKE_CURRENT_SOURCE_DIR} 
         --generate-source
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.gresource.xml 
            ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.ui 
            ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad-menu.ui
)

add_custom_target(${RESOURCES_CT} ALL DEPENDS ${RESOURCES_PN})

file(GLOB_RECURSE CXX_SRCS *.cpp)
file(GLOB_RECURSE C_SRCS *.c)

add_executable(${PROJECT_NAME} "${C_SRCS} ${CXX_SRCS}")

target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES} ${JSONGLIB_LIBRARIES})

message( "# -----------------------------------------------------------------" )
message( "Project         : [${CMAKE_BUILD_TYPE}] ${PROJECT_NAME}" )
message( "Current dir     : ${CMAKE_CURRENT_SOURCE_DIR}" )
message( "Current bin dir : ${CMAKE_CURRENT_BINARY_DIR}" )
message( "Compiler family : ${CMAKE_C_COMPILER_ID}" )
message( "Target Arch     : ${TPLATFORM_ARCHNAME}" )
message( "Compile flags   : ${CMAKE_C_FLAGS}" )
message( "CompileXX flags : ${CMAKE_CXX_FLAGS}" )
message( "Linker flags    : ${CMAKE_LINK_FLAGS}" )
