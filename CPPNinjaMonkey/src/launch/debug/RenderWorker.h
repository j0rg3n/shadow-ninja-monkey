#pragma once
#ifndef RENDERWORKER_H_INCLUDED
#define RENDERWORKER_H_INCLUDED


#include "render/RenderWindow.h"
#include "Entity.h"


// -----------------------------------------------------------------------------


struct RenderWorker
{
	virtual ~RenderWorker() {}

	//!\brief Render one frame.
	virtual void Run() = 0;

	virtual void AddEntity(Entity* pEntity) = 0;

	static RenderWorker* CreateInstance(RenderWindow& renderWindow);
};


#endif // RENDERWORKER_H_INCLUDED