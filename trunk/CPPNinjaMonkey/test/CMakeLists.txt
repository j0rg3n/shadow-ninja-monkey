
SET(UNIT_TEST_SRCS

  CallQueue.cpp

  Point.cpp

  Mediator.cpp
  #HTTPClient.cpp
  JSONRPC.cpp

  Main.cpp
)

#APPLICATION()

#LINK_LIBRARY(unitTests shadowNinjaMath)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp UNIT_TEST_SRCS)

ADD_EXECUTABLE(unitTests ${UNIT_TEST_SRCS})

if(CMAKE_HOST_WIN32)
	# Link with Windows Sockets 2
	# Required by net library.
	target_link_libraries(unitTests wsock32.lib ws2_32.lib)

	# Link with neon
	# Required by net library.
	target_link_libraries(unitTests libneon.lib)
elseif(CMAKE_HOST_UNIX)
	# TODO: Add correct linking for boost::test on linux 
	# (on windows, auto-linking through the header is used)
endif(CMAKE_HOST_WIN32)

TARGET_LINK_LIBRARIES(unitTests shadowNinjaMath shadowNinjaNet shadowNinjaDiag shadowNinjaFramework)