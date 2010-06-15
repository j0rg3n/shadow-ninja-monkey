#include "render/RenderThreadContext.h"
#include "RenderThreadContextInternal.h"

#include <cassert>


// -----------------------------------------------------------------------------


class RenderThreadContext::Impl
{
public:
	Impl(HDC hDC) : m_hRenderContext(NULL), m_hDC(hDC)
	{
		// create and enable the render context (RC)
		m_hRenderContext = wglCreateContext(hDC);
		assert(m_hRenderContext != NULL);

		BOOL bResult = wglMakeCurrent(hDC, m_hRenderContext);
		assert(bResult);
	}


	~Impl()
	{
		BOOL bResult;
		
		bResult = wglMakeCurrent(NULL, NULL);
		assert(bResult);
		
		bResult = wglDeleteContext(m_hRenderContext);
		assert(bResult);
		
		m_hRenderContext = NULL;
	}


	void SetupFont()
	{
		// make the system font the device context's selected font  
		SelectObject(m_hDC, GetStockObject (SYSTEM_FONT)); 
		 
		// create the bitmap display lists  
		// we're making images of glyphs 0 thru 255  
		// the display list numbering starts at 1000, an arbitrary choice  
		wglUseFontBitmaps(m_hDC, 0, 255, 1000); 
	}


private:
	HDC m_hDC;
	HGLRC m_hRenderContext;	
};


// -----------------------------------------------------------------------------


RenderThreadContext::RenderThreadContext(Impl* pImpl) : m_pImpl(pImpl)
{
}


RenderThreadContext::~RenderThreadContext()
{
	delete m_pImpl;
}


void RenderThreadContext::SetupFont()
{
	m_pImpl->SetupFont();
}


// -----------------------------------------------------------------------------


RenderThreadContext* CreateRenderThreadContext(HDC hDC)
{
	return new RenderThreadContext(new RenderThreadContext::Impl(hDC));
}

