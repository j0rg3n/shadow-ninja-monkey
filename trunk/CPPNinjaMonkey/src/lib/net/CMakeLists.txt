

SET(NET_SRCS
  Mediator.cpp
  ${PROJECT_SOURCE_DIR}/include/net/Mediator.h
  #internal/Peer.cpp 
  #Peer.h
)



ADD_LIBRARY(shadowNinjaNet STATIC ${NET_SRCS})

