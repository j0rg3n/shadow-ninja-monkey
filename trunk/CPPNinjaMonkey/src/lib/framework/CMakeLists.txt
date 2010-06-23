

set(FRAMEWORK_SRCS
  ${PROJECT_SOURCE_DIR}/include/framework/AutoResetEvent.h
  CallQueue.cpp
  ${PROJECT_SOURCE_DIR}/include/framework/CallQueue.h
  DispatchThread.cpp
  ${PROJECT_SOURCE_DIR}/include/framework/DispatchThread.h
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp FRAMEWORK_SRCS)

add_library(shadowNinjaFramework STATIC ${FRAMEWORK_SRCS})

