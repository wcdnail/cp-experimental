cmake_minimum_required( VERSION 3.5 )

# collect_info_files function
# Takes collect_info_files - list of non-source files to look for
# Returns INFO_FILES with all files found from the input list.
# Based on macro posted here
# http://lists.qt-project.org/pipermail/qt-creator/2012-August/001191.html [^]
function( collect_info_files )
    list( APPEND _all_found )
    foreach( _it ${ARGN} )
        if( NOT IS_DIRECTORY ${_it} )
            get_filename_component( _path ${_it} ABSOLUTE )
            if( EXISTS ${_path} )
                list( APPEND _all_found ${_it} )
                if( NOT ${_it} MATCHES "^/\\\\..*$;~$" )
                    set_source_files_properties( ${_it} PROPERTIES HEADER_FILE_ONLY TRUE )
                endif()
            endif()
        endif()
    endforeach()
    set( INFO_FILES ${_all_found} PARENT_SCOPE )
endfunction()

if( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
   set( TBITS 64 )
   set( TPLATFORM_ARCHNAME "x86_64" )
else()
   set( TBITS 32 )
   set( TPLATFORM_ARCHNAME "x86" )
endif()

MACRO( ADD_MSVC_PRECOMPILED_HEADER pch_h pch_c src_var )
    GET_FILENAME_COMPONENT( pch_bn ${pch_h} NAME_WE )
    SET( pch_bin "${CMAKE_CURRENT_BINARY_DIR}/${pch_bn}.pch" )
    SET( Sources ${${src_var}})

    SET_SOURCE_FILES_PROPERTIES( ${pch_c}
                    PROPERTIES COMPILE_FLAGS "/Yc\"${pch_h}\" /Fp\"${pch_bin}\""
                               OBJECT_OUTPUTS "${pch_bin}" )
    SET_SOURCE_FILES_PROPERTIES( ${Sources}
    PROPERTIES COMPILE_FLAGS "/Yu\"${pch_h}\" /FI\"${pch_h}\" /Fp\"${pch_bin}\""
               OBJECT_DEPENDS "${pch_bin}" )
    LIST( APPEND ${src_var} ${pch_c} )
ENDMACRO( ADD_MSVC_PRECOMPILED_HEADER )

FUNCTION( _PRE_ADD_PCH src_var _pch_header _pch_source )
   IF( MSVC )
      ADD_MSVC_PRECOMPILED_HEADER( "${_pch_header}" "${_pch_source}" src_var )
   ELSE()
      list( APPEND SOURCES {_pch_source} )
   ENDIF()
ENDFUNCTION( _PRE_ADD_PCH )

FUNCTION( _POST_ADD_PCH _pch_header _pch_source )
   IF( NOT MSVC )
      add_precompiled_header( ${PROJECT_NAME} ${_pch_header} [SOURCE_C ${_pch_source}] )
   ENDIF()
ENDFUNCTION( _POST_ADD_PCH )

