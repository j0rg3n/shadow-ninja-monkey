

set(FRAMEWORK_SRCS
  Dispatcher.cpp
  ${PROJECT_SOURCE_DIR}/include/framework/Dispatcher.h
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp FRAMEWORK_SRCS)

add_library(shadowNinjaFramework STATIC ${FRAMEWORK_SRCS})

