cmake_minimum_required(VERSION 2.6)
project(ShadowNinjaMonkey)

# Remove underscore in _MSVC to enable precompiled headers.
# Note: Currently broken: Causes lots of unresolved externals.
# TODO: Make work.
macro(add_msvc_precompiled_header PrecompiledHeader PrecompiledSource SourcesVar)
  if(_MSVC)
    get_filename_component(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    set(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    set(Sources ${${SourcesVar}})

    set_source_files_properties(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    set_source_files_properties(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
	
    # Add precompiled header to SourcesVar
    list(APPEND ${SourcesVar} ${PrecompiledSource})
    
    # Set macro to enable actual inclusion of headers into the precompiled header
	add_definitions(-DUSING_PRECOMPILED_HEADERS)
  endif(MSVC)
endmacro(add_msvc_precompiled_header)

include_directories(src 
	include)


# include boost for all
set(BOOST_PATH ${PROJECT_SOURCE_DIR}/external/boost_1_42_0
	CACHE PATH "Path to boost 1.42.0.")

include_directories(${PROJECT_SOURCE_DIR}/neon-0.29.3
	${BOOST_PATH})

	
if(CMAKE_HOST_WIN32)
#	set(BOOST_PATH C:/Program\ Files/boost/boost_1_42
#		CACHE PATH "Path to boost 1.42.0.")

#	include_directories(${PROJECT_SOURCE_DIR}/neon-0.29.3
#		${BOOST_PATH})

	# Reduce size and crud in windows headers.
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	
	# Silence warnings about insecure stdlib functions. We already know what they are.
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

elseif(CMAKE_HOST_UNIX)
endif(CMAKE_HOST_WIN32)

add_subdirectory(src)
add_subdirectory(test)