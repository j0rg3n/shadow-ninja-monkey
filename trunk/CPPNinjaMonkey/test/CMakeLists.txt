
SET(UNIT_TEST_SRCS
  Point.cpp
)

#APPLICATION()
#LINK_LIBRARY(unitTests shadowNinjaMath)
ADD_EXECUTABLE(unitTests ${UNIT_TEST_SRCS})
TARGET_LINK_LIBRARIES(unitTests shadowNinjaMath)