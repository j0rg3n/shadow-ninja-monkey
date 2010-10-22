#pragma once
#ifndef JSONRPC_H
#define JSONRPC_H

#include <string>
#include "boost/cstdint.hpp"
#include "boost/property_tree/ptree_fwd.hpp"
#include "boost/function.hpp"

// -----------------------------------------------------------------------------


struct JSONRPC
{
	typedef boost::function<void (const boost::property_tree::ptree&)> ResultCallback;

	static JSONRPC* CreateInstance();
	virtual ~JSONRPC() {};

	virtual bool Connect(std::string sAddress, boost::uint32_t nPort = 80) = 0;
	virtual void Disconnect() = 0;

	virtual bool Call(std::string sFunction, const boost::property_tree::ptree& argProps, ResultCallback fResultCallback) = 0;

	virtual const std::string& GetBoundAddress() const = 0;
	virtual const boost::uint32_t GetBoundPort() const = 0;
};


// -----------------------------------------------------------------------------


#endif // JSONRPC_H
