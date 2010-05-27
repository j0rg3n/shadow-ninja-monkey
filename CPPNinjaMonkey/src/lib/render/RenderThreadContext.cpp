#include "build/PrecompiledHeaders.h"
#include "render/RenderThreadContext.h"
#include "RenderThreadContextInternal.h"

#include <cassert>


// -----------------------------------------------------------------------------


class RenderThreadContext::Impl
{
public:
	Impl(HDC hDC) : m_hRenderContext(NULL)
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


private:
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


// -----------------------------------------------------------------------------


RenderThreadContext* CreateRenderThreadContext(HDC hDC)
{
	return new RenderThreadContext(new RenderThreadContext::Impl(hDC));
}

