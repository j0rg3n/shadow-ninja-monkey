#pragma once
#ifndef WINDOWSRENDERTHREADCONTEXTINTERNAL_H_INCLUDED
#define WINDOWSRENDERTHREADCONTEXTINTERNAL_H_INCLUDED


#include <windows.h>


// -----------------------------------------------------------------------------


struct RenderThreadContext;


// -----------------------------------------------------------------------------


extern RenderThreadContext* CreateRenderThreadContext(HDC hDC);


#endif // WINDOWSRENDERTHREADCONTEXTINTERNAL_H_INCLUDED