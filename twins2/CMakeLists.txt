cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(twins-second)

set(CXX_SRCS 
    stdafx.cpp
    main-twins-second.cpp
)
set(H_SRCS 
    stdafx.h
    main-twins-second.h
)
set(QRC_SRCS )
set(UI_WRAP )
set(MOC_SRCS )
set(OS_SPEC_SRCS )

set(BOOST_DIRECTLY_REQUIRED_LIBRARIES filesystem system)

find_package(Qt5 COMPONENTS Core Widgets Concurrent Network REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

list(APPEND ALL_LIBS 
    Qt5::Core
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::Network
)

if (("${Qt5_VERSION}" VERSION_EQUAL "5.4") OR ("${Qt5_VERSION}" VERSION_GREATER "5.4"))
    set(QT_DEFS
	-DUSE_QOPENGLWIDGET
    )
endif()

find_package(Boost REQUIRED COMPONENTS ${BOOST_DIRECTLY_REQUIRED_LIBRARIES})
list(APPEND ALL_INC_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${Boost_LIBRARIES})

list(REMOVE_DUPLICATES ALL_C_DEFS)
list(REMOVE_DUPLICATES ALL_INC_DIRS)
list(REMOVE_DUPLICATES ALL_LIB_DIRS)
list(REMOVE_DUPLICATES ALL_LIBS)

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

add_executable(${PROJECT_NAME} ${COMMON_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS YES
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_PREFIX_PATH}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${ALL_LIBS}
)

if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
    set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)

if (WIN32)
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS -ENTRY:mainCRTStartup")
        set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${VCPKG_BUILDCONF_DIR}/bin")
    else()
        add_link_options("-Wl,--subsystem,windows")
    endif()
endif()

