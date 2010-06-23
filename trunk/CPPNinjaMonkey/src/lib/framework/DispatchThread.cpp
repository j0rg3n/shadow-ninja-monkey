#include "framework/DispatchThread.h"
#include "framework/CallQueue.h"

#include "boost/thread.hpp"


// ----------------------------------------------------------------------------


using namespace boost;


// ----------------------------------------------------------------------------


class DispatchThread::Impl
{
public:
	Impl() : m_bQuit(false)
	{
		m_dispatchThread = thread(bind(&Impl::DispatchContinuously, this));
	}


	~Impl()
	{
		m_bQuit = true;
		m_callQueue.CancelBlockingCalls();
		m_dispatchThread.join();
	}


	// TODO: Expose only the Enqueue method.
	CallQueue& GetCallQueue() { return m_callQueue; }


private:
	
	void DispatchContinuously()
	{
		while(!m_bQuit)
		{
			const bool bBlock = true;
			m_callQueue.Dispatch(bBlock);
		}
	}


	volatile bool m_bQuit;
	thread m_dispatchThread;
	CallQueue m_callQueue;
};


// -----------------------------------------------------------------------------


DispatchThread::DispatchThread() : m_pImpl(new Impl())
{
}


DispatchThread::~DispatchThread()
{
	delete m_pImpl;
}


CallQueue& DispatchThread::GetCallQueue() 
{ 
	return m_pImpl->GetCallQueue(); 
}