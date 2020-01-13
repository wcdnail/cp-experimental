cmake_minimum_required(VERSION 3.0)

project(launch-pad)

option(USE_GSCHEMA "Compile & install GSettings schema" ON)

if (MSVC)
    set(VCPKGDIR "$ENV{VCPKG_ROOT}")
    set(GLIB_TOOLS_PATH "${VCPKGDIR}/installed/x64-windows/tools/glib")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(VCPKG_BUILDCONF_DIR "${VCPKGDIR}/installed/x64-windows/debug")
    else()
        set(VCPKG_BUILDCONF_DIR "${VCPKGDIR}/installed/x64-windows")
    endif()
    list(APPEND ALL_INC_DIRS 
        "${VCPKGDIR}/installed/x64-windows/include"
    )
    list(APPEND ALL_LIB_DIRS 
        "${VCPKG_BUILDCONF_DIR}/lib"
    )
    list(APPEND ALL_LIBS
        gtk-3.0
        gio-2.0
        glib-2.0
        gmodule-2.0
        gobject-2.0
        gthread-2.0
        gdk-3.0
        gdk_pixbuf-2.0
        epoxy
    )
else()
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

    list(APPEND ALL_LIBS
        ${GTK3_LIBRARIES}
        ${JSONGLIB_LIBRARIES}
        ${EPOXY_LIBRARIES}
    )
    add_definitions(-D_USING_GLIB_JSON)
endif()

list(REMOVE_DUPLICATES ALL_C_DEFS)
list(REMOVE_DUPLICATES ALL_INC_DIRS)
list(REMOVE_DUPLICATES ALL_LIB_DIRS)
list(REMOVE_DUPLICATES ALL_LIBS)

add_definitions(${ALL_C_DEFS})
include_directories(SYSTEM ${ALL_INC_DIRS})
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
)
add_custom_command(
    OUTPUT "${RESOURCES_PN}"
    COMMAND ${GLIB_TOOLS_PATH}/glib-compile-resources
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
        COMMAND ${GLIB_TOOLS_PATH}/glib-compile-schemas
        ARGS --strict
             --targetdir=${SCHEMA_BD}/
             ${CMAKE_CURRENT_SOURCE_DIR}/.
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/wcd.launch.pad.gschema.xml
    )
    add_custom_target(${SCHEMA_CT} ALL DEPENDS ${SCHEMA_PN})
endif()

file(GLOB_RECURSE H_SRCS *.h)
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
    ${H_SRCS}
    ${C_SRCS}
    ${CXX_SRCS}
    ${OS_SPEC_SRCS}
)

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${VCPKG_BUILDCONF_DIR}/bin")
endif()

message("# -----------------------------------------------------------------")
message("VCPKG toolchain file : ${CMAKE_TOOLCHAIN_FILE}")
message("Project              : [${CMAKE_BUILD_TYPE}] ${PROJECT_NAME}")
message("Current dir          : ${CMAKE_CURRENT_SOURCE_DIR}")
message("Current bin dir      : ${CMAKE_CURRENT_BINARY_DIR}")
message("Compiler family      : ${CMAKE_C_COMPILER_ID}")
message("Target Arch          : ${TPLATFORM_ARCHNAME}")
message("Compile flags        : ${CMAKE_C_FLAGS}")
message("CompileXX flags      : ${CMAKE_CXX_FLAGS}")
message("ResCompiler          : ${CMAKE_RC_COMPILER}")
message("ResCompiler flags    : ${CMAKE_RC_FLAGS}")
message("Linker flags         : ${CMAKE_LINK_FLAGS}")
message("# -----------------------------------------------------------------")
message("USE_GSCHEMA          : ${USE_GSCHEMA}")
message("C DEFS               : ${ALL_C_DEFS}")
message("INCLUDES             : ${ALL_INC_DIRS}")
message("LIB DIRS             : ${ALL_LIB_DIRS}")
message("LIBS                 : ${ALL_LIBS}")
message("# -----------------------------------------------------------------")
