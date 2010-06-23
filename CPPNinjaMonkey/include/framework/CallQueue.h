#pragma once
#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED


#include "boost/function.hpp"


// -----------------------------------------------------------------------------


//!\brief Thread-safe callQueue queue.
class CallQueue
{
public:
	typedef boost::function<void()> DispatchEntry;

	CallQueue();
	~CallQueue();

	void Enqueue(DispatchEntry call);

	void Dispatch(bool bBlock = false);
	void CancelBlockingCalls();

	void WaitForPendingCalls(bool bInvokedOnDispatchThread);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // DISPATCHER_H_INCLUDED