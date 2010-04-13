#include "net/Mediator.h"
#include "boost/test/unit_test.hpp"

BOOST_AUTO_TEST_SUITE(MediatorBasic)

BOOST_AUTO_TEST_CASE(Construction)
{
	Mediator mediator("WOHOO!");
	BOOST_CHECK(1 + 1 == 2);
}

BOOST_AUTO_TEST_SUITE_END()