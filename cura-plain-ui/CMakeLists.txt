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

set(SRCS
cura-plain-app.c
cura-plain-settings.c
grect.c
)

add_executable(${PROJECT_NAME} "${SRCS}")

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
