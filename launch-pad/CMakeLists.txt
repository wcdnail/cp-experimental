cmake_minimum_required(VERSION 3.0)

project(launch-pad)

option(USE_GSCHEMA "Compile & install GSettings schema" ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
list(APPEND ALL_C_DEFS   ${GTK3_CFLAGS_OTHER})
list(APPEND ALL_INC_DIRS ${GTK3_INCLUDE_DIRS})
list(APPEND ALL_LIB_DIRS ${GTK3_LIBRARY_DIRS})

pkg_check_modules(JSONGLIB REQUIRED json-glib-1.0)
list(APPEND ALL_C_DEFS   ${JSONGLIB_CFLAGS_OTHER})
list(APPEND ALL_INC_DIRS ${JSONGLIB_INCLUDE_DIRS})
list(APPEND ALL_LIB_DIRS ${JSONGLIB_LIBRARY_DIRS})

pkg_check_modules(EPOXY REQUIRED epoxy)
list(APPEND ALL_C_DEFS   ${EPOXY_CFLAGS_OTHER})
list(APPEND ALL_INC_DIRS ${EPOXY_INCLUDE_DIRS})
list(APPEND ALL_LIB_DIRS ${EPOXY_LIBRARY_DIRS})

list(REMOVE_DUPLICATES ALL_C_DEFS)
add_definitions(${ALL_C_DEFS})

list(REMOVE_DUPLICATES ALL_INC_DIRS)
include_directories(SYSTEM ${ALL_INC_DIRS})

list(REMOVE_DUPLICATES ALL_LIB_DIRS)
link_directories(${ALL_LIB_DIRS})

set(RESOURCES_CT "${PROJECT_NAME}-resources")
set(RESOURCES_PN "${CMAKE_CURRENT_BINARY_DIR}/${RESOURCES_CT}.c")
set(RESOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.gresource.xml 
    ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad-menu.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/sm-star.png
    ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.ico
    ${CMAKE_CURRENT_SOURCE_DIR}/cube-plain.stl
    ${CMAKE_CURRENT_SOURCE_DIR}/glp-fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/glp-vertex.glsl
)
add_custom_command(
    OUTPUT "${RESOURCES_PN}"
    COMMAND glib-compile-resources
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/launch-pad.gresource.xml 
         --target=${RESOURCES_PN} 
         --sourcedir=${CMAKE_CURRENT_SOURCE_DIR} 
         --generate-source
    DEPENDS ${RESOURCES_LIST}
)

if (USE_GSCHEMA)
    set(SCHEMA_CT "gschemas.compiled")
    set(SCHEMA_BD "${CMAKE_CURRENT_BINARY_DIR}/wcd.launch.pad.schema")
    set(SCHEMA_PN "${SCHEMA_BD}/${SCHEMA_CT}")
    add_custom_command(
        OUTPUT "${SCHEMA_PN}"
        COMMAND glib-compile-schemas
        ARGS --strict
             --targetdir=${SCHEMA_BD}/
             ${CMAKE_CURRENT_SOURCE_DIR}/.
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/wcd.launch.pad.gschema.xml
    )
    add_custom_target(${SCHEMA_CT} ALL DEPENDS ${SCHEMA_PN})
endif()

file(GLOB_RECURSE CXX_SRCS *.cpp)
file(GLOB_RECURSE C_SRCS *.c)
list(APPEND C_SRCS ${RESOURCES_PN})

if (WIN32)
    set(CMAKE_RC_COMPILE_OBJECT "${CMAKE_RC_COMPILER} -O coff -I${CMAKE_CURRENT_SOURCE_DIR} <SOURCE> <OBJECT>")
    set(OS_SPEC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/winres.rc)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    else()
        if (MSVC) 
        else()
            add_link_options("-Wl,--subsystem,windows")
        endif()
    endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    set(OS_SPEC_SRCS )
endif()

add_executable(${PROJECT_NAME}
    ${C_SRCS}
    ${CXX_SRCS}
    ${OS_SPEC_SRCS}
)

list(APPEND ALL_LIBS
    ${GTK3_LIBRARIES}
    ${JSONGLIB_LIBRARIES}
    ${EPOXY_LIBRARIES}
)
list(REMOVE_DUPLICATES ALL_LIBS)

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

message( "# -----------------------------------------------------------------" )
message( "Project            : [${CMAKE_BUILD_TYPE}] ${PROJECT_NAME}" )
message( "Current dir        : ${CMAKE_CURRENT_SOURCE_DIR}" )
message( "Current bin dir    : ${CMAKE_CURRENT_BINARY_DIR}" )
message( "Compiler family    : ${CMAKE_C_COMPILER_ID}" )
message( "Target Arch        : ${TPLATFORM_ARCHNAME}" )
message( "Compile flags      : ${CMAKE_C_FLAGS}" )
message( "CompileXX flags    : ${CMAKE_CXX_FLAGS}" )
message( "ResCompiler        : ${CMAKE_RC_COMPILER}" )
message( "ResCompiler flags  : ${CMAKE_RC_FLAGS}" )
message( "Linker flags       : ${CMAKE_LINK_FLAGS}" )
message( "# -----------------------------------------------------------------" )
message( "USE_GSCHEMA        : ${USE_GSCHEMA}" )
message( "C DEFS             : ${ALL_C_DEFS}" )
message( "INCLUDES           : ${ALL_INC_DIRS}" )
message( "LIB DIRS           : ${ALL_LIB_DIRS}" )
message( "LIBS               : ${ALL_LIBS}" )
message( "# -----------------------------------------------------------------" )
