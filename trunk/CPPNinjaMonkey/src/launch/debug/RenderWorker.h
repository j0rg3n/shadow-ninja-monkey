#pragma once
#ifndef RENDERWORKER_H_INCLUDED
#define RENDERWORKER_H_INCLUDED


#include "render/RenderWindow.h"


// -----------------------------------------------------------------------------


class RenderWorker
{
public:
	RenderWorker(RenderWindow& renderWindow);
	~RenderWorker();

	//void AddTask(Dispatcher::DispatchEntry task);
	//!\brief Render one frame.
	void Run();

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // RENDERWORKER_H_INCLUDED