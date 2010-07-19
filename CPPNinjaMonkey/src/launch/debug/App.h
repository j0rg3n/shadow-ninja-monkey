#pragma once
#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED


#include "render/RenderWindow.h"


// -----------------------------------------------------------------------------


struct App 
{
	virtual ~App() {}
	
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	//! \brief Call this when main thread is idling.
	virtual void OnIdle() = 0;

	virtual boost::signals2::connection ConnectClosedSlot(const RenderWindow::ClosedSignal::slot_type& slot) = 0;

	static App* CreateInstance();
};


#endif // APP_H_INCLUDED