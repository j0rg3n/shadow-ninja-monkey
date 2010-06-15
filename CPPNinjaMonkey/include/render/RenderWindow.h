#pragma once
#ifndef RENDERWINDOW_H_INCLUDED
#define RENDERWINDOW_H_INCLUDED


#include "boost/signal.hpp"


// -----------------------------------------------------------------------------


class RenderThreadContext;


// -----------------------------------------------------------------------------


class RenderWindow
{
public:
	RenderWindow();
	~RenderWindow();

	void Init();
	void Shutdown();

	void Swap();

	int Width() const;
	int Height() const;

	RenderThreadContext* CreateRenderThreadContext();

	boost::signal<void (int, int)>& SizeChanged();
	boost::signal<void ()>& Closed();

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // RENDERWINDOW_H_INCLUDED