
SET(UNIT_TEST_SRCS
  Point.cpp
  Mediator.cpp
  Main.cpp
)

#APPLICATION()

#LINK_LIBRARY(unitTests shadowNinjaMath)

link_directories(${BOOST_PATH}/lib)
ADD_EXECUTABLE(unitTests ${UNIT_TEST_SRCS})

# TODO: Add correct linking for boost::test on linux 
# (on windows, auto-linking through the header is used)
TARGET_LINK_LIBRARIES(unitTests shadowNinjaMath shadowNinjaNet)