#include "framework/Dispatcher.h"
#include "boost/test/unit_test.hpp"
#include <iostream>


#include "boost/bind.hpp"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// ----------------------------------------------------------------------------


BOOST_AUTO_TEST_SUITE(DispatcherBasic)


BOOST_AUTO_TEST_CASE(Construction)
{
	Dispatcher c;
}


static void AppendToString(string& sString, const string& sPart)
{
	sString.append(sPart);
}


struct Appender
{
	Appender(string& sString) : m_sString(sString)
	{
	}

	void Append(const string& sPart)
	{
		m_sString.append(sPart);
	}

	string& m_sString;
};


BOOST_AUTO_TEST_CASE(MakeCalls)
{
	string sResult;

	Dispatcher c;
	c.Enqueue(bind(&AppendToString, ref(sResult), "one, "));
	c.Enqueue(bind(&AppendToString, ref(sResult), "two, "));
	c.Enqueue(bind(&AppendToString, sResult, "lost to a copy, "));

	Appender a(sResult);
	c.Enqueue(bind(&Appender::Append, a, "three."));

	BOOST_CHECK(sResult == "");

	c.Dispatch();

	BOOST_CHECK(sResult == "one, two, three.");
}


BOOST_AUTO_TEST_SUITE_END()