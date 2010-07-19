
set(INPUT_SRCS
  ${PROJECT_SOURCE_DIR}/include/input/Input.h
  ${PROJECT_SOURCE_DIR}/include/input/windows/WindowsMessageInput.h
  windows/WindowsMessageInput.cpp
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp INPUT_SRCS)

add_library(shadowNinjaInput STATIC ${INPUT_SRCS})
