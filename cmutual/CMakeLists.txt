cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(cmutual)

if (BUILD_SHARED_LIBS AND WIN32)
    add_definitions(-D_DEFINE_DLL_EXPORT)
endif()

set(CXX_SRCS 
    stdafx.cpp
    trace-it.cpp
)
set(H_SRCS 
    stdafx.h
    win-imp-exp.h
    trace-it.h
)
#set(QRC_SRCS )
#set(UI_WRAP )
#set(MOC_SRCS )
set(OS_SPEC_SRCS )

set(BOOST_DIRECTLY_REQUIRED_LIBRARIES system)
find_package(Boost REQUIRED COMPONENTS ${BOOST_DIRECTLY_REQUIRED_LIBRARIES})
list(APPEND ALL_INC_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${Boost_LIBRARIES})

list(REMOVE_DUPLICATES ALL_C_DEFS)
list(REMOVE_DUPLICATES ALL_INC_DIRS)
list(REMOVE_DUPLICATES ALL_LIB_DIRS)
list(REMOVE_DUPLICATES ALL_LIBS)

#qt5_wrap_cpp(MOC_SRCS ${MOC_H_SRCS} OPTIONS "-bstdafx.h")
#qt5_wrap_ui(UI_WRAP ${UI_SRCS})

source_group("" FILES ${CXX_SRCS} ${C_SRCS} ${H_SRCS})
#source_group("moc" FILES ${MOC_SRCS})
#source_group("ui" FILES ${UI_SRCS} ${UI_WRAP})
#source_group("res" FILES ${QRC_SRCS})
source_group("win32" FILES ${OS_SPEC_SRCS})
source_group("pch" FILES 
    stdafx.h
    stdafx.cpp
)
source_group("misc" FILES 
    CMakeLists.txt
)

list(APPEND CMUTUAL_SRCS 
    ${CXX_SRCS}
    ${C_SRCS}
    ${H_SRCS}
    ${QRC_SRCS}
    ${UI_WRAP}
    ${MOC_SRCS}
    ${OS_SPEC_SRCS}
)

set(VCPKG_APPLOCAL_DEPS OFF)
add_library(${PROJECT_NAME} ${LIB_STATIC_SHARED} ${CMUTUAL_SRCS})

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
