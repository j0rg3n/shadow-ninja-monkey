cmake_minimum_required(VERSION 2.6)
PROJECT(ShadowNinjaMonkey)

set(BOOST_PATH C:/Program\ Files/boost/boost_1_42
	CACHE PATH "Path to boost 1.42.0.")

INCLUDE_DIRECTORIES(src 
	include 
	${BOOST_PATH}
	)

add_subdirectory(src)
add_subdirectory(test)