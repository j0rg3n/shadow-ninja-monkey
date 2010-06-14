#include "RenderWorker.h"
#include "render/RenderThreadContext.h"

// TODO: Find out how to include GL.h without including windows.h, if possible.
#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include <GL.h>

#include "boost/scoped_ptr.hpp"


// ----------------------------------------------------------------------------


using namespace boost;


// ----------------------------------------------------------------------------


class RenderWorker::Impl
{
public:
	Impl(RenderWindow& renderWindow) : 
		renderThreadContext(renderWindow.CreateRenderThreadContext())
	{
	}


	void Run()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.1f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glPushMatrix();
		{
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -30.0f);
			
			glPushMatrix();
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				glTranslatef(0, 0, 0);
				glScalef(1.0f, 4.0f, 1.0f);

				glPushMatrix();
				{
					glBegin(GL_POLYGON);
					{
						/*      This is the top face*/
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(0.0f, 0.0f, -1.0f);
						glVertex3f(-1.0f, 0.0f, -1.0f);
						glVertex3f(-1.0f, 0.0f, 0.0f);

						/*      This is the front face*/
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(-1.0f, 0.0f, 0.0f);
						glVertex3f(-1.0f, -1.0f, 0.0f);
						glVertex3f(0.0f, -1.0f, 0.0f);

						/*      This is the right face*/
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(0.0f, -1.0f, 0.0f);
						glVertex3f(0.0f, -1.0f, -1.0f);
						glVertex3f(0.0f, 0.0f, -1.0f);

						/*      This is the left face*/
						glVertex3f(-1.0f, 0.0f, 0.0f);
						glVertex3f(-1.0f, 0.0f, -1.0f);
						glVertex3f(-1.0f, -1.0f, -1.0f);
						glVertex3f(-1.0f, -1.0f, 0.0f);

						/*      This is the bottom face*/
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(0.0f, -1.0f, -1.0f);
						glVertex3f(-1.0f, -1.0f, -1.0f);
						glVertex3f(-1.0f, -1.0f, 0.0f);

						/*      This is the back face*/
						glVertex3f(0.0f, 0.0f, 0.0f);
						glVertex3f(-1.0f, 0.0f, -1.0f);
						glVertex3f(-1.0f, -1.0f, -1.0f);
						glVertex3f(0.0f, -1.0f, -1.0f);
					}
					glEnd();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	
private:
	scoped_ptr<RenderThreadContext> renderThreadContext;
};


// -----------------------------------------------------------------------------


RenderWorker::RenderWorker(RenderWindow& renderWindow) : m_pImpl(new Impl(renderWindow))
{
}


RenderWorker::~RenderWorker()
{
	delete m_pImpl;
}


void RenderWorker::Run()
{
	m_pImpl->Run();
}