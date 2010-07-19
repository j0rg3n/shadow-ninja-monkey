#pragma once
#ifndef WINDOWSRENDERWINDOW_H_INCLUDED
#define WINDOWSRENDERWINDOW_H_INCLUDED


#include "../RenderWindow.h"
#include <windows.h>


// -----------------------------------------------------------------------------


struct WindowsRenderWindow : RenderWindow
{
	typedef boost::signals2::signal<void (HWND, UINT, WPARAM, LPARAM)> WindowsMessageSignal;

	virtual boost::signals2::connection ConnectWindowsMessageSlot(const WindowsMessageSignal::slot_type& slot) = 0;
};


#endif // WINDOWSRENDERWINDOW_H_INCLUDED