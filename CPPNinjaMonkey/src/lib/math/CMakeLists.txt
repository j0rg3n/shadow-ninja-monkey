

SET(MATH_SRCS
  Point.cpp Point.h
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp MATH_SRCS)

ADD_LIBRARY(shadowNinjaMath STATIC ${MATH_SRCS})

