#pragma once
#ifndef RENDERTHREADCONTEXT_H_INCLUDED
#define RENDERTHREADCONTEXT_H_INCLUDED


struct RenderThreadContext
{
	virtual ~RenderThreadContext() {}

	virtual void SetupFont() = 0;
};


#endif // RENDERTHREADCONTEXT_H_INCLUDED