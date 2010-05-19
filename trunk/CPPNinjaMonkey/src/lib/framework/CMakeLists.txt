

set(FRAMEWORK_SRCS
  Dispatcher.cpp
  ${PROJECT_SOURCE_DIR}/include/framework/Dispatcher.h
  #internal/Peer.cpp 
  #Peer.h
)

#if(CMAKE_HOST_WIN32)
	# Reduce size and crud in windows headers.
#	add_definitions(-DWIN32_LEAN_AND_MEAN)
	
	# Silence warnings about insecure stdlib functions. We already know what they are.
#	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#elseif(CMAKE_HOST_UNIX)
#endif(CMAKE_HOST_WIN32)

add_library(shadowNinjaFramework STATIC ${FRAMEWORK_SRCS})

