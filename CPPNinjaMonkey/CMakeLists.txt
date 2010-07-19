cmake_minimum_required(VERSION 2.6)
project(ShadowNinjaMonkey)

# Remove underscore in _MSVC to enable precompiled headers.
# Note: Currently broken: Causes lots of unresolved externals.
# TODO: Make work.
macro(add_msvc_precompiled_header PrecompiledHeader PrecompiledSource SourcesVar)
  if(MSVC)
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
  endif()
endmacro(add_msvc_precompiled_header)

include_directories(src 
	include)
	

# include boost for all
SET(Boost_ADDITIONAL_VERSIONS "1.42.0")
SET(BOOST_ROOT ${PROJECT_SOURCE_DIR}/external/boost_1_42_0)
find_package(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif()

#Include neon
include_directories(${PROJECT_SOURCE_DIR}/neon-0.29.3)
link_directories(${PROJECT_SOURCE_DIR}/neon-0.29.3)
	
if(CMAKE_HOST_WIN32)
	# Reduce size and crud in windows headers.
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	
	# Silence warnings about insecure stdlib functions. 
	# We already know what they are.
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	# Silence warnings about insecure C++ library functions. 
	# (They're recommending "secure iterators". The nerve!)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif(CMAKE_HOST_WIN32)

add_subdirectory(src)
add_subdirectory(test)