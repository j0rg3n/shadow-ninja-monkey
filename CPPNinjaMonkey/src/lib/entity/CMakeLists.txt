
set(ENTITY_SRCS
  Entity.cpp
  ${PROJECT_SOURCE_DIR}/include/entity/Entity.h
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp ENTITY_SRCS)

add_library(shadowNinjaEntity STATIC ${ENTITY_SRCS})
