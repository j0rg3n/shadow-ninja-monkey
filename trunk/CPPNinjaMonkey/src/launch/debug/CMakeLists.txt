#
# Debug launcher; simple, platform-independent launcher for debugging purposes 
# only.
#

set(DEBUG_LAUNCH_SRCS
  App.cpp
  App.h
  windows/Main.cpp
  RenderWorker.cpp
  RenderWorker.h
  ${PROJECT_SOURCE_DIR}/include/build/PrecompiledHeaders.h 
)

# Comment this line to disable precompiled headers for this project only.
# Should be done once in a while.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp DEBUG_LAUNCH_SRCS)
	
add_executable(debugLaunch ${DEBUG_LAUNCH_SRCS})

if(CMAKE_HOST_WIN32)
	# Link with Windows Sockets 2
	# Required by net library.
	target_link_libraries(debugLaunch wsock32.lib ws2_32.lib)

	# Link with OpenGL
	target_link_libraries(debugLaunch Opengl32.lib glu32.lib)

	# Link with neon
	# Required by net library.
	target_link_libraries(debugLaunch libneon.lib)
elseif(CMAKE_HOST_UNIX)
	# TODO: Add correct linking for boost::test on linux 
	# (on windows, auto-linking through the header is used)
endif(CMAKE_HOST_WIN32)

target_link_libraries(debugLaunch 
	shadowNinjaMath 
	shadowNinjaNet 
	shadowNinjaFramework 
	shadowNinjaRender
	shadowNinjaGame
	shadowNinjaDiag
	shadowNinjaInput)