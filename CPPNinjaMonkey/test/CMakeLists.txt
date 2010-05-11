
SET(UNIT_TEST_SRCS
  Point.cpp
  Mediator.cpp
  #HTTPClient.cpp
  JSONRPC.cpp
  Main.cpp
)

#APPLICATION()

#LINK_LIBRARY(unitTests shadowNinjaMath)

link_directories(${BOOST_PATH}/lib 
	${PROJECT_SOURCE_DIR}/neon-0.29.3)

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

TARGET_LINK_LIBRARIES(unitTests shadowNinjaMath shadowNinjaNet)