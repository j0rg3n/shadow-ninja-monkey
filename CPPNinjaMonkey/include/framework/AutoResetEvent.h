#pragma once
#ifndef AUTORESETEVENT_H_INCLUDED
#define AUTORESETEVENT_H_INCLUDED


#include "boost/thread/condition_variable.hpp"
#include "boost/thread/locks.hpp"
#include "boost/thread/mutex.hpp"


// -----------------------------------------------------------------------------


class AutoResetEvent
{
public:
	AutoResetEvent() : m_bSet(false)
	{
	}


	void Set() 
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_bSet = true;
		m_setCondition.notify_one();
	}


	void Wait() 
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		while(!m_bSet)
		{
			m_setCondition.wait(lock);
		}

		m_bSet = false;
	}


private:
	volatile bool m_bSet;
	boost::condition_variable m_setCondition;
	boost::mutex m_mutex;
};


#endif // AUTORESETEVENT_H_INCLUDED