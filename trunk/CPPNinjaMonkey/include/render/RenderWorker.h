#pragma once
#ifndef RENDERWORKER_H_INCLUDED
#define RENDERWORKER_H_INCLUDED


#include <vector>

#include "render/RenderWindow.h"
#include "entity/Entity.h"


// -----------------------------------------------------------------------------


struct RenderWorker
{
	virtual ~RenderWorker() {}

	//!\brief Render one frame.
	virtual void Run() = 0;

	//!\brief Set game entities to render.
	virtual void SetEntities(std::vector<Entity*>& entities) = 0;

	static RenderWorker* CreateInstance(RenderWindow& renderWindow);
};


#endif // RENDERWORKER_H_INCLUDED