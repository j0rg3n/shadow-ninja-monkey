#include "lib/net/JSONRPC.h"
#include "boost/test/unit_test.hpp"
#include "lib/net/Socket.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include <iostream>


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost::property_tree;


// ----------------------------------------------------------------------------


BOOST_AUTO_TEST_SUITE(JSONRPCBasic)


BOOST_AUTO_TEST_CASE(Construction)
{
	Socket::InitNetwork();
	{
		JSONRPC c;
	}
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_CASE(ConnectToPage)
{
	Socket::InitNetwork();
	{
		JSONRPC c;

		c.Connect("fabeljet.com");
	}
	Socket::DeinitNetwork();
}


struct ResultCapturer
{
	ResultCapturer(ptree& result) : m_result(result)
	{
	}

	void operator()(const ptree& result)
	{
		m_result = result;
	}

	ptree& m_result;
};


BOOST_AUTO_TEST_CASE(MakeSimpleCall)
{
	Socket::InitNetwork();
	{
		JSONRPC c;

		c.Connect("fabeljet.com");

		ptree arguments;
		arguments.put("name", "Sanchez");
		cout << "Arguments: " << endl;
		xml_parser::write_xml(cout, arguments);
		cout << endl;

		ptree result;
		bool bResult = c.Call("helloWorld", arguments, ResultCapturer(result));
		BOOST_CHECK(bResult);

		cout << "Result: " << endl;
		xml_parser::write_xml(cout, result);
		cout << endl;

		boost::optional<string> error = result.get_optional<string>("error");
		BOOST_CHECK(!error.is_initialized());

		boost::optional<string> message = result.get_optional<string>("message");
		BOOST_CHECK(message.is_initialized() && message.get() == "Hello, Sanchez!");
	}
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_SUITE_END()