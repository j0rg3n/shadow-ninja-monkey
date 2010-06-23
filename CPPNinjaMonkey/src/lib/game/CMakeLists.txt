
set(GAME_SRCS
  GameLoop.cpp
  GameLoop.h
  GameNetworkPacketTranslator.cpp
  GameNetworkPacketTranslator.h
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp GAME_SRCS)

add_library(shadowNinjaGame STATIC ${GAME_SRCS})
