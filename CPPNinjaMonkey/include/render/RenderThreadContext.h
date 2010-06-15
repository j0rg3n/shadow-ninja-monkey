#pragma once
#ifndef RENDERTHREADCONTEXT_H_INCLUDED
#define RENDERTHREADCONTEXT_H_INCLUDED


class RenderThreadContext
{
public:
	class Impl;

	RenderThreadContext(Impl* pImpl);
	~RenderThreadContext();

	void SetupFont();

private:
	Impl* m_pImpl;
};


#endif // RENDERTHREADCONTEXT_H_INCLUDED