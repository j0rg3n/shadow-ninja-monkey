#include "lib/net/JSONRPC.h"
#include "boost/test/unit_test.hpp"
#include "lib/net/Socket.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/scoped_ptr.hpp"

#include <iostream>
#include <conio.h>


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost;
using namespace boost::property_tree;


// ----------------------------------------------------------------------------


BOOST_AUTO_TEST_SUITE(JSONRPCBasic)


BOOST_AUTO_TEST_CASE(Construction)
{
	Socket::InitNetwork();
	{
		scoped_ptr<JSONRPC> c(JSONRPC::CreateInstance());
	}
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_CASE(ConnectToPage)
{
	Socket::InitNetwork();
	{
		scoped_ptr<JSONRPC> c(JSONRPC::CreateInstance());

		c->Connect("fabeljet.com");
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
		scoped_ptr<JSONRPC> c(JSONRPC::CreateInstance());

		c->Connect("fabeljet.com");

		ptree arguments;
		arguments.put("name", "Sanchez");
		cout << "Arguments: " << endl;
		xml_parser::write_xml(cout, arguments);
		cout << endl;

		ptree result;
		bool bResult = c->Call("helloWorld", arguments, ResultCapturer(result));
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


BOOST_AUTO_TEST_CASE(MakeMediatorCall)
{
	Socket::InitNetwork();
	{
		scoped_ptr<JSONRPC> c(JSONRPC::CreateInstance());

		c->Connect("fabeljet.com");
		ptree arguments;
		arguments.put("name", "Mediatee");

		// TODO: Call mediate stuff instead of helloWorld!
		ptree result;
		bool bResult = c->Call("helloWorld", arguments, ResultCapturer(result));
		BOOST_CHECK(bResult);

		boost::optional<string> error = result.get_optional<string>("error");
		BOOST_CHECK(!error.is_initialized());

		boost::optional<string> message = result.get_optional<string>("message");
		BOOST_CHECK(message.is_initialized() && message.get() == "Hello, Mediatee!");

		cout << "Connected to server, bound to " << c->GetBoundAddress() << ":" << c->GetBoundPort() << endl;

		cout << "Press any key to continue...";
		_getch();
	}
	Socket::DeinitNetwork();
}


BOOST_AUTO_TEST_SUITE_END()