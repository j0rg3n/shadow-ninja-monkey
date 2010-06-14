#pragma once
#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED


#include "boost/function.hpp"


// -----------------------------------------------------------------------------


//!\brief Thread-safe dispatcher queue.
class Dispatcher
{
public:
	typedef boost::function<void()> DispatchEntry;

	Dispatcher();
	~Dispatcher();

	void Enqueue(DispatchEntry call);
	void Dispatch();

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // DISPATCHER_H_INCLUDED