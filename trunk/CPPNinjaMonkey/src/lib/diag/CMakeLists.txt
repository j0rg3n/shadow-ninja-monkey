
set(DIAG_SRCS
  ${PROJECT_SOURCE_DIR}/include/diag/Trace.h
  Trace.cpp
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp DIAG_SRCS)

add_library(shadowNinjaDiag STATIC ${DIAG_SRCS})
