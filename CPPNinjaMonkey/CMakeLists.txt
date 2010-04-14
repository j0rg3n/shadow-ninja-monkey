cmake_minimum_required(VERSION 2.6)
PROJECT(ShadowNinjaMonkey)

INCLUDE_DIRECTORIES(src 
	include)
	
if(CMAKE_HOST_WIN32)
	set(BOOST_PATH C:/Program\ Files/boost/boost_1_42
		CACHE PATH "Path to boost 1.42.0.")
	include_directories(${PROJECT_SOURCE_DIR}/neon-0.29.3
		${BOOST_PATH})
elseif(CMAKE_HOST_UNIX)
endif(CMAKE_HOST_WIN32)

add_subdirectory(src)
add_subdirectory(test)