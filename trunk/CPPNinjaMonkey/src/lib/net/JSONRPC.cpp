#include "JSONRPC.h"

#include "Neon.h"
#include "boost/smart_ptr/scoped_ptr.hpp"
#include "boost/property_tree_vs2010/ptree.hpp"
#include "boost/property_tree_vs2010/json_parser.hpp"
#include "boost/thread/mutex.hpp"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace boost::property_tree;
using namespace std;


// ----------------------------------------------------------------------------


typedef void (InternalResultCallback)(string sBody);


// ----------------------------------------------------------------------------


class CallInfo
{
public:
	CallInfo(HTTPSession& httpSession, string sPath, JSONRPC::ResultCallback fResultCallback) :
		m_sPath(sPath),
		m_fResultCallback(fResultCallback),
		m_httpRequest(httpSession, "GET", sPath.c_str())
	{
	}


	void Run()
	{
		ptree resultProps;
		if (m_httpRequest.Dispatch())
		{
			istringstream resultStream(m_httpRequest.GetResult());
			try
			{
				json_parser::read_json(resultStream, resultProps);
			}
			catch (const json_parser::json_parser_error& e)
			{
				resultProps.clear();
				resultProps.put("error", e.what());
				resultProps.put("path", m_sPath);
				resultProps.put("content", resultStream.str());
			}
		}
		else
		{
			// TODO: Include error details from HTTP request.
			resultProps.clear();
			resultProps.put("error", "Yes, indeed.");
		}

		m_fResultCallback(resultProps);
	}


private:
	string m_sPath;
	JSONRPC::ResultCallback m_fResultCallback;
	HTTPRequest m_httpRequest;
};


// ----------------------------------------------------------------------------


class JSONRPC::Impl
{
public:
	Impl()
	{
	}


	bool Connect(string sAddress, uint32_t nPort)
	{
		assert(NULL == m_pSession.get());
		m_pSession.reset(new HTTPSession("http", sAddress.c_str(), nPort));
		return true;
	}


	bool Call(string sFunction, const ptree& argProps, ResultCallback fResultCallback)
	{
		assert(NULL != m_pSession.get());

		lock_guard<mutex> lock(m_callsMutex);

		++m_callID;

		ostringstream jsonStream;
		json_parser::write_json(jsonStream, argProps);

		ostringstream pathStream;
		pathStream << "/snm/ajax.php";
		pathStream << "?id=" << m_callID;
		pathStream << "&name=";
		HTTPSession::WriteURLEscaped(pathStream, sFunction);
		pathStream << "&args=";
		HTTPSession::WriteURLEscaped(pathStream, jsonStream.str());
		
		CallInfo* pCallInfo = new CallInfo(*m_pSession, pathStream.str(), fResultCallback);

		// TODO: Add call to call queue on separate thread where the call to Run is made.
		pCallInfo->Run();
		delete pCallInfo;
		return true;
	}


private:
	typedef map<int, CallInfo*> CallMap;
	typedef uint32_t CallID;

	scoped_ptr<HTTPSession> m_pSession;

	mutex m_callsMutex;
	CallID m_callID;
	CallMap m_calls;
};


// ----------------------------------------------------------------------------


JSONRPC::JSONRPC() : m_pImpl(new Impl())
{
}


JSONRPC::~JSONRPC()
{
	delete m_pImpl;
}


bool JSONRPC::Connect(std::string sAddress, boost::uint32_t nPort)
{
	return m_pImpl->Connect(sAddress, nPort);
}


void JSONRPC::Disconnect()
{
	//m_pImpl->Disconnect();
}


bool JSONRPC::Call(std::string sFunction, const boost::property_tree::ptree& argProps, ResultCallback fResultCallback)
{
	return m_pImpl->Call(sFunction, argProps, fResultCallback);
}
