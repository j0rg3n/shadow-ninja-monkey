find_package(OpenGL)

SET(RENDER_SRCS
  RenderThreadContext.cpp
  RenderThreadContextInternal.h
  ${PROJECT_SOURCE_DIR}/include/render/RenderThreadContext.h
  RenderWindow.cpp
  ${PROJECT_SOURCE_DIR}/include/render/RenderWindow.h
)

# Comment this line to disable precompiled headers.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp RENDER_SRCS)

ADD_LIBRARY(shadowNinjaRender STATIC ${RENDER_SRCS})

