#pragma once
#ifndef JSONRPC_H
#define JSONRPC_H

#include <string>
#include "boost/cstdint.hpp"
#include "boost/property_tree/ptree_fwd.hpp"
#include "boost/function.hpp"

// -----------------------------------------------------------------------------


class JSONRPC
{
public:
	typedef boost::function<void (const boost::property_tree::ptree&)> ResultCallback;

	JSONRPC();
	~JSONRPC();

	bool Connect(std::string sAddress, boost::uint32_t nPort = 80);
	void Disconnect();

	bool Call(std::string sFunction, const boost::property_tree::ptree& argProps, ResultCallback fResultCallback);

private:
	class Impl;

	Impl* m_pImpl;
};


// -----------------------------------------------------------------------------


#endif // JSONRPC_H
