#include "lib/net/HTTPClient.h"
#include "boost/test/unit_test.hpp"
#include "lib/net/Socket.h"

BOOST_AUTO_TEST_SUITE(HTTPClientBasic)


BOOST_AUTO_TEST_CASE(Construction)
{
	Socket::InitNetwork();
	{
		HTTPClient c;
	}
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_CASE(ConnectToGoogle)
{
	Socket::InitNetwork();
	{
		HTTPClient c;

		c.Connect("google.com", 80);
	}
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_CASE(GetGooglePage)
{
	Socket::InitNetwork();
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
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_SUITE_END()