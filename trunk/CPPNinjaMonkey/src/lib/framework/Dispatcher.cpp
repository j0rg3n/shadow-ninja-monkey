#include "framework/Dispatcher.h"

#include <list>
#include <vector>
#include "boost/thread/mutex.hpp"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class Dispatcher::Impl
{
public:
	void Enqueue(DispatchEntry call)
	{
		lock_guard<mutex> lock(m_queueMutex);
		m_queue.push_back(call);
	}


	void Dispatch()
	{
		vector<DispatchEntry> queueCopy;

		{
			lock_guard<mutex> lock(m_queueMutex);

			queueCopy.reserve(m_queue.size());
			queueCopy.assign(m_queue.begin(), m_queue.end());
		}

		for (vector<DispatchEntry>::const_iterator i = queueCopy.begin(); i != queueCopy.end(); ++i)
		{
			(*i)();
		}
	}


private:
	mutex m_queueMutex;
	list<DispatchEntry> m_queue;
};


// -----------------------------------------------------------------------------


Dispatcher::Dispatcher() : m_pImpl(new Impl())
{
}


Dispatcher::~Dispatcher()
{
	delete m_pImpl;
}


void Dispatcher::Enqueue(DispatchEntry call)
{
	m_pImpl->Enqueue(call);
}


void Dispatcher::Dispatch()
{
	m_pImpl->Dispatch();
}