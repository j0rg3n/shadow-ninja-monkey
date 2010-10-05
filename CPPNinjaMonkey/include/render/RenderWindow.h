#pragma once
#ifndef RENDERWINDOW_H_INCLUDED
#define RENDERWINDOW_H_INCLUDED


#include "boost/signals2/signal.hpp"


// -----------------------------------------------------------------------------


struct RenderThreadContext;


// -----------------------------------------------------------------------------


struct RenderWindow
{
	typedef boost::signals2::signal<void (int, int)> SizeChangedSignal;
	typedef boost::signals2::signal<void ()> ClosedSignal;

	virtual ~RenderWindow() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Close() = 0;

	virtual void Swap() = 0;

	virtual int Width() const = 0;
	virtual int Height() const = 0;

	virtual RenderThreadContext* CreateRenderThreadContext() = 0;

	virtual boost::signals2::connection ConnectSizeChangedSlot(const SizeChangedSignal::slot_type& slot) = 0;
	virtual boost::signals2::connection ConnectClosedSlot(const ClosedSignal::slot_type& slot) = 0;

	static RenderWindow* CreateInstance();
};


#endif // RENDERWINDOW_H_INCLUDED