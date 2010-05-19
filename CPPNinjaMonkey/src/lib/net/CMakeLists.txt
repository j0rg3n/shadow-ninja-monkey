

SET(NET_SRCS
  Mediator.cpp
  ${PROJECT_SOURCE_DIR}/include/net/Mediator.h
  HTTPClient.cpp
  HTTPClient.h
  Socket.cpp
  Socket.h
  JSONRPC.cpp
  JSONRPC.h
  Neon.h
  Neon.cpp
  #internal/Peer.cpp 
  #Peer.h
)

ADD_LIBRARY(shadowNinjaNet STATIC ${NET_SRCS})

