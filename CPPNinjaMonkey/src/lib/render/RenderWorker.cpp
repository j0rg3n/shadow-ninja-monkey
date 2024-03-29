#include "render/RenderWorker.h"
#include "render/RenderThreadContext.h"
#include "render/GL.h"

#include "diag/Trace.h"

#include <string>

#include "boost/scoped_ptr.hpp"

#include "entity/Entity.h"


// ----------------------------------------------------------------------------


using namespace boost;
using namespace std;


// ----------------------------------------------------------------------------


class RenderWorkerImpl : public RenderWorker
{
public:
	RenderWorkerImpl(RenderWindow& renderWindow) : 
		m_pRenderThreadContext(renderWindow.CreateRenderThreadContext()),
		m_nAngle(0)
	{
		SetViewport(renderWindow.Width(), renderWindow.Height());
		renderWindow.ConnectSizeChangedSlot(boost::bind(&RenderWorkerImpl::OnSizeChanged, this, _1, _2));

		m_pRenderThreadContext->SetupFont();
	}


	virtual ~RenderWorkerImpl()
	{
	}


	virtual void Run()
	{
		m_nAngle += 0.08f;

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();

		glPushMatrix();
		{
			DrawEntities();
		}
		glPopMatrix();
	}


	virtual void SetEntities(std::vector<Entity*>& entities)
	{
		// TODO: Clone entities to fix encapsulation.
		m_entities.reserve(entities.size());			
		m_entities.assign(entities.begin(), entities.end());
	}


private:
	void OnSizeChanged(int nWidth, int nHeight)
	{
		SetViewport(nWidth, nHeight);
	}


	void SetViewport(int nWidth, int nHeight)
	{
		if (nHeight == 0)
		{
			nHeight = 1;
		}

		// TODO: Move away: Perspective matrix should be set in render code, not here.
		TRACE("Updating projection for new viewport size %dx%d", nWidth, nHeight);
		glViewport(0, 0, nWidth, nHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(54.0f, (GLfloat) nWidth / (GLfloat) nHeight, 1.0f, 1000.0f);
	}


	void DrawEntities()
	{
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -10.0f);

		for (EntityList::const_iterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
		{
			string& sLabel = (*entity)->m_sName;
			Geometry* pGeometry = (*entity)->m_pGeometry;
			if (pGeometry)
			{
				glPushMatrix();
				{
					glTranslatef((*entity)->m_pos.x, (*entity)->m_pos.y, 0);

					// Draw text. 
					glColor3f(1.0f, 0.0f, 0.0f);
					glRasterPos3f(0, 0, 0);
					glListBase(1000); //< 1000 is an arbitrary constant used in SetupFont().
					glCallLists(sLabel.size(), GL_UNSIGNED_BYTE, sLabel.c_str()); 

					glColor3f(0.0f, 0.0f, 1.0f);

					glBegin(GL_TRIANGLES);
					for (Geometry::TriangleList::const_iterator t = pGeometry->m_triangles.begin(); t != pGeometry->m_triangles.end(); ++t)
					{
						for (int i = 0; i < 3; ++i)
						{
							glVertex2f(t->m_points[i].x, t->m_points[i].y);
						}
					}
					glEnd();
				}
				glPopMatrix();
			}
		}
	}


	void DrawDebugStuff()
	{
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -30.0f);
			
		glPushMatrix();
		{
			glColor3f(1.0f, 0.0f, 0.0f);

			// Draw text. 1000 is an arbitrary constant used in SetupFont().
			string msg = "A box with text attached";
			glRasterPos3f(0, 0, 0);
			glListBase(1000); 
			glCallLists(msg.size(), GL_UNSIGNED_BYTE, msg.c_str()); 

			glColor3f(0.0f, 0.0f, 1.0f);

			glTranslatef(0, 0, 0);

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
	

	typedef vector<Entity*> EntityList;

	scoped_ptr<RenderThreadContext> m_pRenderThreadContext;
	EntityList m_entities;
	float m_nAngle;
};


// -----------------------------------------------------------------------------


RenderWorker* RenderWorker::CreateInstance(RenderWindow& renderWindow)
{
	return new RenderWorkerImpl(renderWindow);
}
