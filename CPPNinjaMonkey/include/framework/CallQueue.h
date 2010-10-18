#pragma once
#ifndef CALL_QUEUE_H_INCLUDED
#define CALL_QUEUE_H_INCLUDED


#include "boost/function.hpp"


// -----------------------------------------------------------------------------


//!\brief Thread-safe call queue.
class CallQueue
{
public:
	typedef boost::function<void()> DispatchEntry;

	CallQueue();
	~CallQueue();

	void Enqueue(DispatchEntry call);

	//!\param bBlock If true, will block until there is anything queued. If false,
	//!it will return immediately if the queue is empty.
	void Dispatch(bool bBlock = false);
	void CancelBlockingCalls();

	void WaitForPendingCalls(bool bInvokedOnDispatchThread);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // CALL_QUEUE_H_INCLUDED