#include "framework/CallQueue.h"
#include "boost/test/unit_test.hpp"
#include <iostream>


#include "boost/bind.hpp"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;


// ----------------------------------------------------------------------------


BOOST_AUTO_TEST_SUITE(CallQueueBasic)


BOOST_AUTO_TEST_CASE(Construction)
{
	CallQueue c;
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

	CallQueue c;
	c.Enqueue(boost::bind(&AppendToString, boost::ref(sResult), "one, "));
	c.Enqueue(boost::bind(&AppendToString, boost::ref(sResult), "two, "));
	c.Enqueue(boost::bind(&AppendToString, sResult, "lost to a copy, "));

	Appender a(sResult);
	c.Enqueue(boost::bind(&Appender::Append, a, "three."));

	BOOST_CHECK(sResult == "");

	c.Dispatch();

	BOOST_CHECK(sResult == "one, two, three.");
}


BOOST_AUTO_TEST_SUITE_END()