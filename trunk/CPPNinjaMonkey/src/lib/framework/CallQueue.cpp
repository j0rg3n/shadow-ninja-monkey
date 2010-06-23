#include "framework/CallQueue.h"

#include <list>
#include <vector>
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "framework/AutoResetEvent.h"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class CallQueue::Impl
{
public:
	Impl() : m_bCancelBlocking(false)
	{
	}


	void Enqueue(DispatchEntry call)
	{
		unique_lock<mutex> lock(m_queueMutex);
		m_queue.push_back(call);
		m_queueNotEmptyCondition.notify_all();
	}


	void Dispatch(bool bBlock)
	{
		vector<DispatchEntry> queueCopy;

		{
			unique_lock<mutex> lock(m_queueMutex);

			while(m_queue.size() == 0 && !m_bCancelBlocking)
			{
				m_queueNotEmptyCondition.wait(lock);
			}

			if(m_bCancelBlocking)
			{
				// TODO: Adjust this code to support calling Dispatch(true) from multiple 
				// threads simultaneously to get thread pool behavior. Current code
				// may cause some threads not to get unblocked.
				m_bCancelBlocking = false;
				return;
			}

			queueCopy.reserve(m_queue.size());
			queueCopy.assign(m_queue.begin(), m_queue.end());
			m_queue.clear();
		}

		for (vector<DispatchEntry>::const_iterator i = queueCopy.begin(); i != queueCopy.end(); ++i)
		{
			(*i)();
		}
	}


	void CancelBlockingCalls()
	{
		unique_lock<mutex> lock(m_queueMutex);
		m_bCancelBlocking = true;
		m_queueNotEmptyCondition.notify_all();
	}


	//! Making sure all internal processing has stopped in an object using a 
	//! callQueue:
	//! First, ensure that all users of the object have been shut down or detached. 
	//! Then make sure that internal processing has been shut down in the object. 
	//! Then ensure that all enqueued dispatch calls have been flushed
	//! by calling WaitForPendingCalls.
	void WaitForPendingCalls(bool bInvokedOnDispatchThread)
	{
		if(bInvokedOnDispatchThread)
		{
			// CallQueue is running on the same thread as the Wait call.
			bool bCompleted;
			Enqueue(bind(&Impl::SetToTrue, ref(bCompleted)));
			while(!bCompleted)
			{
				const bool bBlock = false;
				Dispatch(bBlock);
			}
		}
		else
		{
			// CallQueue is running on a different thread than the Wait call.
			AutoResetEvent completedEvent;
			Enqueue(bind(&AutoResetEvent::Set, &completedEvent));
			completedEvent.Wait();
		}
	}


private:
	static void SetToTrue(bool& bValue)
	{
		bValue = true;
	}


	mutex m_queueMutex;
	list<DispatchEntry> m_queue;
	bool m_bCancelBlocking;
	condition_variable m_queueNotEmptyCondition;
};


// -----------------------------------------------------------------------------


CallQueue::CallQueue() : m_pImpl(new Impl())
{
}


CallQueue::~CallQueue()
{
	delete m_pImpl;
}


void CallQueue::Enqueue(DispatchEntry call)
{
	m_pImpl->Enqueue(call);
}


void CallQueue::Dispatch(bool bBlock)
{
	m_pImpl->Dispatch(bBlock);
}


void CallQueue::CancelBlockingCalls()
{
	m_pImpl->CancelBlockingCalls();
}


void CallQueue::WaitForPendingCalls(bool bInvokedOnDispatchThread)
{
	m_pImpl->WaitForPendingCalls(bInvokedOnDispatchThread);
}
