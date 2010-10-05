#include "render/windows/WindowsRenderWindow.h"
#include "WindowsRenderThreadContextInternal.h"

#include <cassert>

#include <windows.h>


// ----------------------------------------------------------------------------


using namespace boost;
using namespace boost::signals2;


// ----------------------------------------------------------------------------


class WindowsRenderWindowImpl : public WindowsRenderWindow
{
public:
	WindowsRenderWindowImpl() : m_hDC(NULL), m_windowClass(0), m_hWnd(NULL), m_hInstance(NULL)
	{
	}


	virtual void Init()
	{
		m_hInstance = (HINSTANCE) GetModuleHandle(NULL);
		m_windowClass = RegisterWindowClass(m_hInstance);
		m_hWnd = CreateMainWindow(m_windowClass, m_hInstance, this);

		EnableOpenGL(m_hWnd, &m_hDC);
	}


	virtual void Shutdown()
	{
		DisableOpenGL(m_hWnd, m_hDC);
		m_hDC = NULL;

		UnregisterWindowClass(m_windowClass, m_hInstance);
		m_windowClass = 0;
	}


	virtual void Close()
	{
		m_quit();
		DestroyWindow(m_hWnd);
	}


	virtual void Swap()
	{
		SwapBuffers(m_hDC);
	}


	virtual int Width() const { return m_nWidth; }
	virtual int Height() const { return m_nHeight; }


	virtual ~WindowsRenderWindowImpl()
	{
		// TODO: Do something useful when we've decided on exception safety stuff.
	}


	virtual RenderThreadContext* CreateRenderThreadContext()
	{
		assert(m_hDC != NULL);
		return ::CreateRenderThreadContext(m_hDC);
	}


	virtual boost::signals2::connection ConnectSizeChangedSlot(const RenderWindow::SizeChangedSignal::slot_type& slot) 
	{ 
		return m_sizeChanged.connect(slot); 
	}


	virtual boost::signals2::connection ConnectClosedSlot(const RenderWindow::ClosedSignal::slot_type& slot) 
	{ 
		return m_quit.connect(slot); 
	}


	virtual boost::signals2::connection ConnectWindowsMessageSlot(const WindowsMessageSignal::slot_type& slot)
	{
		return m_windowsMessage.connect(slot);
	}


private:
	static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsRenderWindowImpl* pThis = GetThis(hwnd);

		switch (uMsg)
		{
		case WM_CREATE:
			{
				CREATESTRUCT* pCreateStruct = (CREATESTRUCT*) lParam;
				SetUserDataFromCreateStruct(hwnd, pCreateStruct);

				pThis = GetThis(hwnd);
				assert(pThis != NULL);
				pThis->m_sizeChanged(pCreateStruct->cx, pCreateStruct->cy);
			}
			break;

		case WM_CLOSE:
			pThis->m_quit();
			break;

		case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);
				pThis->m_nWidth = width;
				pThis->m_nHeight = height;
				pThis->m_sizeChanged(width, height);
			}
            break;
		}

		if(pThis != NULL)
		{
			pThis->m_windowsMessage(hwnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


	//!\brief Will return NULL until SetUserDataFromCreateStruct is called.
	static WindowsRenderWindowImpl* GetThis(HWND hwnd)
	{
		WindowsRenderWindowImpl* pThis = (WindowsRenderWindowImpl*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
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
	RenderWindow::SizeChangedSignal m_sizeChanged;
	RenderWindow::ClosedSignal m_quit;
	WindowsRenderWindow::WindowsMessageSignal m_windowsMessage;
};


// ----------------------------------------------------------------------------


RenderWindow* RenderWindow::CreateInstance()
{
	return new WindowsRenderWindowImpl();
}
