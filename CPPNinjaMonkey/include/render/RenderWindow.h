#pragma once
#ifndef RENDERWINDOW_H_INCLUDED
#define RENDERWINDOW_H_INCLUDED


#include "boost/signals2/signal.hpp"


// -----------------------------------------------------------------------------


class RenderThreadContext;


// -----------------------------------------------------------------------------


class RenderWindow
{
public:
	typedef boost::signals2::signal<void (int, int)> SizeChangedSignal;
	typedef boost::signals2::signal<void ()> ClosedSignal;

	RenderWindow();
	~RenderWindow();

	void Init();
	void Shutdown();

	void Swap();

	int Width() const;
	int Height() const;

	RenderThreadContext* CreateRenderThreadContext();

	boost::signals2::connection ConnectSizeChangedSlot(const SizeChangedSignal::slot_type& slot);
	boost::signals2::connection ConnectClosedSlot(const ClosedSignal::slot_type& slot);

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // RENDERWINDOW_H_INCLUDED