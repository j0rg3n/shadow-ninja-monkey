#pragma once
#ifndef RENDERTHREADCONTEXTINTERNAL_H_INCLUDED
#define RENDERTHREADCONTEXTINTERNAL_H_INCLUDED


// TODO: Move this whole file into a win32-specific subfolder.
#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS


// -----------------------------------------------------------------------------


class RenderThreadContext;


// -----------------------------------------------------------------------------


RenderThreadContext* CreateRenderThreadContext(HDC hDC);


#endif // RENDERTHREADCONTEXTINTERNAL_H_INCLUDED