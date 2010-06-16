

SET(NET_SRCS
  Mediator.cpp
  ${PROJECT_SOURCE_DIR}/include/net/Mediator.h
  HTTPClient.cpp
  HTTPClient.h
  PeerClient.cpp
  PeerClient.h
  PeerServerConnectionListener.cpp
  PeerServerConnectionListener.h
  PeerServer.cpp
  PeerServer.h
  PeerServerSession.cpp
  PeerServerSession.h
  Socket.cpp
  Socket.h
  JSONRPC.cpp
  JSONRPC.h
  Neon.h
  Neon.cpp
  #internal/Peer.cpp 
  #Peer.h
)

# Comment this line to disable precompiled headers.
add_msvc_precompiled_header(build/PrecompiledHeaders.h ${PROJECT_SOURCE_DIR}/src/build/PrecompiledHeaders.cpp NET_SRCS)

ADD_LIBRARY(shadowNinjaNet STATIC ${NET_SRCS})

