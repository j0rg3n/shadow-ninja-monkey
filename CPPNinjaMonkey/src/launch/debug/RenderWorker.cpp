#include "RenderWorker.h"
#include "render/RenderThreadContext.h"

// TODO: Find out how to include GL.h without including windows.h, if possible.
#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include <gl/GL.h>

#include <string>

#include "boost/scoped_ptr.hpp"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class RenderWorker::Impl
{
public:
	Impl(RenderWindow& renderWindow) : 
		m_pRenderThreadContext(renderWindow.CreateRenderThreadContext()),
		m_nAngle(0)
	{
		m_pRenderThreadContext->SetupFont();
	}


	void Run()
	{
		m_nAngle += 0.08f;

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

                // Draw text. 1000 is an arbitrary constant used in SetupFont().
				string msg = "A box with text attached";
				glRasterPos3f(0, 0, 0);
				glListBase(1000); 
				glCallLists(msg.size(), GL_UNSIGNED_BYTE, msg.c_str()); 

				glRotatef(m_nAngle * 1.00f, 0, 0, 1);
				glRotatef(m_nAngle * 1.13f, 0, 1, 0);
				glRotatef(m_nAngle * 1.07f, 1, 0, 0);
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
	scoped_ptr<RenderThreadContext> m_pRenderThreadContext;
	float m_nAngle;
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