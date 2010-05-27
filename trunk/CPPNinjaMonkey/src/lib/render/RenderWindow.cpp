#include "build/PrecompiledHeaders.h"
#include "render/RenderWindow.h"
#include "RenderThreadContextInternal.h"

#include <cassert>

// TODO: Move this whole file into a win32-specific subfolder.
#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS


// ----------------------------------------------------------------------------


class RenderWindow::Impl
{
public:
	Impl() : m_hDC(NULL), m_windowClass(0), m_hWnd(NULL), m_hInstance(NULL)
	{
	}

	void Init()
	{
		m_hInstance = (HINSTANCE) GetModuleHandle(NULL);
		m_windowClass = RegisterWindowClass(m_hInstance);
		m_hWnd = CreateMainWindow(m_windowClass, m_hInstance);

		EnableOpenGL(m_hWnd, &m_hDC);
	}

	void Shutdown()
	{
		DisableOpenGL(m_hWnd, m_hDC);
		m_hDC = NULL;

		UnregisterWindowClass(m_windowClass, m_hInstance);
		m_windowClass = 0;
	}


	~Impl()
	{
		// TODO: Do something useful when we've decided on exception safety stuff.
	}


	RenderThreadContext* CreateRenderThreadContext()
	{
		assert(m_hDC != NULL);
		return ::CreateRenderThreadContext(m_hDC);
	}


private:
	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			PostMessage(hwnd, WM_QUIT, 0, 0);
			break;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


	static ATOM RegisterWindowClass(HINSTANCE hInstance)
	{
		WNDCLASSEX windowClassDescription;
		ZeroMemory(&windowClassDescription, sizeof(windowClassDescription));
		windowClassDescription.cbSize = sizeof(windowClassDescription);
		windowClassDescription.lpfnWndProc = &MainWindowProc;
		windowClassDescription.hInstance = hInstance;
		windowClassDescription.hbrBackground = (HBRUSH) GetStockObject(DKGRAY_BRUSH);
		windowClassDescription.lpszClassName = "{589CDC59-183D-43d0-A6EA-34B294EC53C4}";

		ATOM windowClass = RegisterClassEx(&windowClassDescription);
		assert(windowClass != NULL);

		return windowClass;
	}


	static void UnregisterWindowClass(ATOM windowClass, HINSTANCE hInstance)
	{
		UnregisterClass((LPCTSTR) windowClass, hInstance);
	}


	static HWND CreateMainWindow(ATOM windowClass, HINSTANCE hInstance)
	{
		DWORD nExStyle = 0;
		const char* pszWindowName = "Foo";
		DWORD nStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		int nX = CW_USEDEFAULT;
		int nY = CW_USEDEFAULT;
		int nWidth = 800;
		int nHeight = 600;
		HWND hWndParent = GetDesktopWindow();
		HMENU hMenu = NULL;
		void* pParam = NULL;

		HWND hwnd = CreateWindowEx(nExStyle, (LPCTSTR) windowClass, pszWindowName, nStyle, nX, nY, nWidth, nHeight, hWndParent, hMenu, hInstance, pParam);
		assert(hwnd != NULL);

		BOOL bWasVisible = ShowWindow(hwnd, SW_SHOWNORMAL);
		return hwnd;
	}


	static void EnableOpenGL(HWND hWnd, HDC* hDC)
	{
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		int iFormat;

		// get the device context (DC)
		*hDC = GetDC(hWnd);

		// set the pixel format for the DC
		ZeroMemory( &pixelFormatDescriptor, sizeof( pixelFormatDescriptor ) );
		pixelFormatDescriptor.nSize = sizeof( pixelFormatDescriptor );
		pixelFormatDescriptor.nVersion = 1;
		pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER;
		pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
		pixelFormatDescriptor.cColorBits = 24;
		pixelFormatDescriptor.cDepthBits = 16;
		pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

		iFormat = ChoosePixelFormat(*hDC, &pixelFormatDescriptor);

		SetPixelFormat(*hDC, iFormat, &pixelFormatDescriptor);
	}


	static void DisableOpenGL(HWND hWnd, HDC hDC)
	{
		ReleaseDC(hWnd, hDC);
	}


	HDC m_hDC;
	ATOM m_windowClass;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
};


// -----------------------------------------------------------------------------


RenderWindow::RenderWindow() : m_pImpl(new Impl())
{
}


RenderWindow::~RenderWindow()
{
	delete m_pImpl;
}


void RenderWindow::Init()
{
	m_pImpl->Init();
}


void RenderWindow::Shutdown()
{
	m_pImpl->Shutdown();
}


RenderThreadContext* RenderWindow::CreateRenderThreadContext()
{
	return m_pImpl->CreateRenderThreadContext();
}
