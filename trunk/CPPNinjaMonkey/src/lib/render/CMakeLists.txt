find_package(OpenGL)

SET(RENDER_SRCS
  windows/WindowsRenderThreadContext.cpp
  windows/WindowsRenderThreadContextInternal.h
  ${PROJECT_SOURCE_DIR}/include/render/RenderThreadContext.h
  windows/WindowsRenderWindow.cpp
  ${PROJECT_SOURCE_DIR}/include/render/RenderWindow.h
  ${PROJECT_SOURCE_DIR}/include/render/windows/WindowsRenderWindow.h
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp RENDER_SRCS)

ADD_LIBRARY(shadowNinjaRender STATIC ${RENDER_SRCS})

