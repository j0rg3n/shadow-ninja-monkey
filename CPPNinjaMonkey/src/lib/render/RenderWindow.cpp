#include "render/RenderWindow.h"
#include "RenderThreadContextInternal.h"

#include <cassert>

// TODO: Move this whole file into a win32-specific subfolder.
#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS


// ----------------------------------------------------------------------------


using namespace boost;
using namespace boost::signals2;


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
		m_hWnd = CreateMainWindow(m_windowClass, m_hInstance, this);

		EnableOpenGL(m_hWnd, &m_hDC);
	}


	void Shutdown()
	{
		DisableOpenGL(m_hWnd, m_hDC);
		m_hDC = NULL;

		UnregisterWindowClass(m_windowClass, m_hInstance);
		m_windowClass = 0;
	}


	void Swap()
	{
		SwapBuffers(m_hDC);
	}


	int Width() const { return m_nWidth; }
	int Height() const { return m_nHeight; }


	~Impl()
	{
		// TODO: Do something useful when we've decided on exception safety stuff.
	}


	RenderThreadContext* CreateRenderThreadContext()
	{
		assert(m_hDC != NULL);
		return ::CreateRenderThreadContext(m_hDC);
	}


	boost::signals2::signal<void (int, int)>& SizeChanged() { return m_sizeChanged; }
	boost::signals2::signal<void ()>& Closed() { return m_quit; }

private:
	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
			{
				CREATESTRUCT* pCreateStruct = (CREATESTRUCT*) lParam;
				SetUserDataFromCreateStruct(hwnd, pCreateStruct);
				GetThis(hwnd)->m_sizeChanged(pCreateStruct->cx, pCreateStruct->cy);
			}
			break;

		case WM_CLOSE:
			GetThis(hwnd)->m_quit();
			break;

		case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);
				Impl* pThis = GetThis(hwnd);
				pThis->m_nWidth = width;
				pThis->m_nHeight = height;
				pThis->m_sizeChanged(width, height);
			}
            break;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


	//!\brief Do not call before calling SetUserDataFromCreateStruct
	static Impl* GetThis(HWND hwnd)
	{
		Impl* pThis = (Impl*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
		assert(pThis != NULL);
		return pThis;
	}


	static void SetUserDataFromCreateStruct(HWND hwnd, CREATESTRUCT* pCreateStruct)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) pCreateStruct->lpCreateParams);
		// Force cached window long values to be flushed.
		// (See SetWindowLongPtr or SetWindowPos documentation for more.)
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
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


	static HWND CreateMainWindow(ATOM windowClass, HINSTANCE hInstance, void* pParam)
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


	int m_nWidth;
	int m_nHeight;
	HDC m_hDC;
	ATOM m_windowClass;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	signal<void (int, int)> m_sizeChanged;
	signal<void ()> m_quit;
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


void RenderWindow::Swap()
{
	m_pImpl->Swap();
}


int RenderWindow::Width() const
{
	return m_pImpl->Width();
}


int RenderWindow::Height() const
{
	return m_pImpl->Height();
}


RenderThreadContext* RenderWindow::CreateRenderThreadContext()
{
	return m_pImpl->CreateRenderThreadContext();
}


boost::signals2::signal<void (int, int)>& RenderWindow::SizeChanged()
{
	return m_pImpl->SizeChanged();
}


boost::signals2::signal<void ()>& RenderWindow::Closed()
{
	return m_pImpl->Closed();
}
