#pragma once
#ifndef DISPATCHTHREAD_H_INCLUDED
#define DISPATCHTHREAD_H_INCLUDED


#include "framework/CallQueue.h"


// -----------------------------------------------------------------------------


class DispatchThread
{
public:
	DispatchThread();
	~DispatchThread();

	CallQueue& GetCallQueue();

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // DISPATCHTHREAD_H_INCLUDED