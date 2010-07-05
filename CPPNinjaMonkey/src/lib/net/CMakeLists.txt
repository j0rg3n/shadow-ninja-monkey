

SET(NET_SRCS
  Mediator.cpp
  ${PROJECT_SOURCE_DIR}/include/net/Mediator.h
  HTTPClient.cpp
  HTTPClient.h
  NetworkPacket.cpp
  ${PROJECT_SOURCE_DIR}/include/net/NetworkPacket.h
  NetworkPacketMarshaller.cpp
  NetworkPacketMarshaller.h
  PeerServerConnectionListener.cpp
  PeerServerConnectionListener.h
  PeerServer.cpp
  ${PROJECT_SOURCE_DIR}/include/net/PeerServer.h
  PeerServerSession.cpp
  PeerServerSession.h
  Socket.cpp
  Socket.h
  JSONRPC.cpp
  JSONRPC.h
  Neon.h
  Neon.cpp
)

# Comment this line to disable precompiled headers for this project only.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp NET_SRCS)

ADD_LIBRARY(shadowNinjaNet STATIC ${NET_SRCS})

