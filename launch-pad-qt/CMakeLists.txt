cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

include("../cmake/pch.cmake")
include("../cmake/misc.cmake")

project(launch-pad-qt)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_DIRECTLY_REQUIRED_LIBRARIES filesystem system)

find_package(OpenGL REQUIRED QUIET)
list(APPEND ALL_LIBS ${OPENGL_LIBRARIES})

find_package(Freetype 2.4.9 REQUIRED QUIET)
list(APPEND ALL_INC_DIRS ${FREETYPE_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${FREETYPE_LIBRARIES})

find_package(GLEW REQUIRED)
list(APPEND ALL_INC_DIRS ${GLEW_INCLUDE_DIR})
list(APPEND ALL_LIBS ${GLEW_LIBRARY})

find_package(Qt5 COMPONENTS Core Widgets Multimedia OpenGL Concurrent Network REQUIRED QUIET)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

list(APPEND ALL_LIBS 
    Qt5::Core
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::OpenGL
    Qt5::Concurrent
    Qt5::Network
)

find_package(Boost 1.36 REQUIRED COMPONENTS ${BOOST_DIRECTLY_REQUIRED_LIBRARIES} QUIET)
list(APPEND ALL_INC_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${Boost_LIBRARIES})

list(REMOVE_DUPLICATES ALL_C_DEFS)
list(REMOVE_DUPLICATES ALL_INC_DIRS)
list(REMOVE_DUPLICATES ALL_LIB_DIRS)
list(REMOVE_DUPLICATES ALL_LIBS)
include_directories(${ALL_INC_DIRS})

file(GLOB_RECURSE CXX_SRCS *.cpp)
file(GLOB_RECURSE C_SRCS *.c)
file(GLOB_RECURSE H_SRCS *.h)
file(GLOB_RECURSE UI_SRCS *.ui)
file(GLOB_RECURSE QRC_SRCS *.qrc)

file(GLOB_RECURSE MOC_H_SRCS 
    app.h
    winmain.h
    logbox.h
)
qt5_wrap_cpp(MOC_SRCS ${MOC_H_SRCS} OPTIONS "-bstdafx.h")
qt5_wrap_ui(UI_WRAP ${UI_SRCS})

if (WIN32)
    set(CMAKE_RC_COMPILE_OBJECT "${CMAKE_RC_COMPILER} -O coff -I${CMAKE_CURRENT_SOURCE_DIR} <SOURCE> <OBJECT>")
    set(OS_SPEC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/winres.rc)
endif()

source_group("" FILES ${CXX_SRCS} ${C_SRCS} ${H_SRCS})
source_group("moc" FILES ${MOC_SRCS})
source_group("ui" FILES ${UI_SRCS} ${UI_WRAP})
source_group("res" FILES ${QRC_SRCS})
source_group("win32" FILES ${OS_SPEC_SRCS})
source_group("pch" FILES 
    stdafx.h
    stdafx.cpp
)
source_group("misc" FILES 
    CMakeLists.txt
)

list(APPEND COMMON_SOURCES 
    ${CXX_SRCS}
    ${C_SRCS}
    ${H_SRCS}
    ${QRC_SRCS}
    ${UI_WRAP}
    ${MOC_SRCS}
    ${OS_SPEC_SRCS}
)

_PRE_ADD_PCH(SOURCES stdafx.h stdafx.cpp)
add_executable(${PROJECT_NAME} ${COMMON_SOURCES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
    set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
else()
   _POST_ADD_PCH(stdafx.h stdafx.cpp)
endif(MSVC)

if (WIN32)
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS -ENTRY:mainCRTStartup")
    else()
        add_link_options("-Wl,--subsystem,windows")
    endif()
endif()

message(STATUS "# -----------------------------------------------------------------")
message(STATUS "Project              : [${CMAKE_BUILD_TYPE}] ${PROJECT_NAME}")
message(STATUS "Current dir          : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Current bin dir      : ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Compiler family      : ${CMAKE_C_COMPILER_ID}")
message(STATUS "Target Arch          : ${TPLATFORM_ARCHNAME}")
message(STATUS "Compile flags        : ${CMAKE_C_FLAGS}")
message(STATUS "CompileXX flags      : ${CMAKE_CXX_FLAGS}")
message(STATUS "ResCompiler          : ${CMAKE_RC_COMPILER}")
message(STATUS "ResCompiler flags    : ${CMAKE_RC_FLAGS}")
message(STATUS "Linker flags         : ${CMAKE_LINK_FLAGS}")
message(STATUS "# -----------------------------------------------------------------")
message(STATUS "Boost reqs           : ${BOOST_DIRECTLY_REQUIRED_LIBRARIES}")
message(STATUS "Boost                : ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
message(STATUS "Boost include dir    : ${Boost_INCLUDE_DIRS}")
message(STATUS "Freetype             : ${FREETYPE_VERSION_STRING}")
message(STATUS "Glew include dir     : ${GLEW_INCLUDE_DIR}")
message(STATUS "Glew library         : ${GLEW_LIBRARY}")
message(STATUS "Qt5                  : ${Qt5_VERSION}")
message(STATUS "# -----------------------------------------------------------------")
