#include "lib/net/HTTPClient.h"
#include "boost/test/unit_test.hpp"
#include "neon/ne_session.h"

BOOST_AUTO_TEST_SUITE(HTTPClientBasic)


BOOST_AUTO_TEST_CASE(NeonSessionTest)
{
	HTTPClient c;

	ne_session* pSession = ne_session_create("http", "google.com", 80);
	BOOST_CHECK(pSession != NULL);

	ne_session_destroy(pSession);
}


BOOST_AUTO_TEST_CASE(Construction)
{
	HTTPClient c;
}


BOOST_AUTO_TEST_CASE(ConnectToGoogle)
{
	HTTPClient c;

	c.Connect("google.com", 80);
}


BOOST_AUTO_TEST_CASE(GetGooglePage)
{
	HTTPClient c;

	bool bResult = c.Connect("google.com", 80);
	BOOST_CHECK(bResult);

	std::string sResult;
	bResult = c.Get("/search?q=http&ie=utf-8&oe=utf-8&aq=t&rls=org.mozilla:nb-NO:official&client=firefox-a", sResult);
	BOOST_CHECK(bResult);

	puts(sResult.c_str());
	getchar();
}


BOOST_AUTO_TEST_SUITE_END()