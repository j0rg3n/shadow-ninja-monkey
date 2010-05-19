#include "build/PrecompiledHeaders.h"
#include <iostream>

#include <gl/gl.h>
#include <gl/glu.h>

#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

// -----------------------------------------------------------------------------


using namespace std;


// -----------------------------------------------------------------------------


void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	// get the device context (DC)
	*hDC = GetDC( hWnd );

	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, iFormat, &pfd );

	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hRC );
}


void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}


static ATOM s_windowClass = 0;
static HWND s_hwnd = NULL;
static HINSTANCE s_hInstance = NULL;


LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostMessage(hwnd, WM_QUIT, 0, 0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


static void RegisterWindowClass()
{
	WNDCLASSEX windowClassDescription;
	ZeroMemory(&windowClassDescription, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.lpfnWndProc = &MainWindowProc;
	windowClassDescription.hInstance = s_hInstance;
	windowClassDescription.hbrBackground = (HBRUSH) GetStockObject(DKGRAY_BRUSH);
	windowClassDescription.lpszClassName = "{589CDC59-183D-43d0-A6EA-34B294EC53C4}";

	s_windowClass = RegisterClassEx(&windowClassDescription);
	assert(s_windowClass != NULL);
}


static void UnregisterWindowClass()
{
	UnregisterClass((LPCTSTR) s_windowClass, s_hInstance);
}


static void CreateMainWindow()
{
	DWORD nExStyle = 0;
	const char* pszWindowName = "Foo";
	DWORD nStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	int nX = CW_USEDEFAULT;
	int nY = CW_USEDEFAULT;
	int nWidth = 800;
	int nHeight = 600;
	HWND hwndParent = GetDesktopWindow();
	HMENU hMenu = NULL;
	HINSTANCE hInstance = s_hInstance;
	void* pParam = NULL;

	s_hwnd = CreateWindowEx(nExStyle, (LPCTSTR) s_windowClass, pszWindowName, nStyle, nX, nY, nWidth, nHeight, hwndParent, hMenu, hInstance, pParam);
	assert(s_hwnd != NULL);

	BOOL bWasVisible = ShowWindow(s_hwnd, SW_SHOWNORMAL);
}


// -----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
	cout << "Hello world." << endl;

	s_hInstance = (HINSTANCE) GetModuleHandle(NULL);

	RegisterWindowClass();
	CreateMainWindow();

	MSG msg;
	
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterWindowClass();

	return msg.wParam;
}