#include "GameLoop.h"

#include "entity/Entity.h"
#include "diag/Trace.h"
#include "boost/date_time/posix_time/posix_time.hpp"


// ----------------------------------------------------------------------------


using namespace std;
using namespace boost::posix_time;


// ----------------------------------------------------------------------------


class GameLoopImpl : public GameLoop
{
public:
	GameLoopImpl() : m_movementVector(0, 0)
	{
		m_entities.push_back(new Entity(MakeSquare(), Point(0, 0)));
		m_entities.push_back(new Entity(MakeSquare(), Point(1, 1)));
		m_entities.push_back(new Entity(MakeSquare(), Point(2, 2)));

		m_entities[0]->m_sName = "Light#1";
		m_entities[1]->m_sName = "Light#2";
		m_entities[2]->m_sName = "Nïnja#1";
	}


	virtual ~GameLoopImpl()
	{
		for (vector<Entity*>::iterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
		{
			delete *entity;
		}
	}


	virtual void Run()
	{
		static ptime startTime = microsec_clock::universal_time();
		ptime now = microsec_clock::universal_time();
		
		float t = (now - startTime).total_milliseconds() / 1000.0f;

		int index = 0;
		for (vector<Entity*>::iterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
		{
			if (index == 0 || index == 1)
			{
				float angle = index * 3.14f + t * (2 * 3.14f / 4);
				float r = 2.0f;
				(*entity)->m_pos.x = cosf(angle) * r;
				(*entity)->m_pos.y = sinf(angle) * r;
			}
			else
			{
				(*entity)->m_pos.x += m_movementVector.x * t * 0.001f;
				(*entity)->m_pos.y += m_movementVector.y * t * 0.001f;
			}

			++index;
		}
	}


	virtual std::vector<Entity*>& GetEntities()
	{
		return m_entities;
	}


	virtual void OnPeerJoined(SessionID nSessionID)
	{
		TRACE("Peer %d joined.", nSessionID);
	}


	virtual void OnPeerLeft(SessionID nSessionID)
	{
		TRACE("Peer %d left.", nSessionID);
	}


	virtual void OnPeerUpdatePosition(SessionID nSessionID, boost::int32_t x, boost::int32_t y)
	{
		TRACE("Peer %d position: (%d, %d)", nSessionID, x, y);
	}	


	virtual void OnAxisUpdate(const std::string& sAxisName, float nValue)
	{
		TRACE("%s = %.2f", sAxisName.c_str(), nValue);
	}


	virtual void OnButtonUpdate(const std::string& sButtonName, bool bPressed)
	{
		TRACE("%s %s", sButtonName.c_str(), bPressed ? "down" : "up");

		if (sButtonName == "RIGHT")
		{
			m_movementVector.x += bPressed ? .5f : -.5f;
		}
		else if (sButtonName == "LEFT")
		{
			m_movementVector.x -= bPressed ? .5f : -.5f;
		}		
		else if (sButtonName == "UP")
		{
			m_movementVector.y += bPressed ? .5f : -.5f;
		}		
		else if (sButtonName == "DOWN")
		{
			m_movementVector.y -= bPressed ? .5f : -.5f;
		}
	}


private:
	// TODO: Move to some sort of resource framework thingy.
	static Geometry* MakeSquare()
	{
		Triangle t;

		Geometry* pSquare = new Geometry();

		t.m_points[0] = Point(-.5f, -.5f);
		t.m_points[1] = Point(-.5f,  .5f);
		t.m_points[2] = Point( .5f,  .5f);
		pSquare->m_triangles.push_back(t);

		t.m_points[0] = Point(-.5f, -.5f);
		t.m_points[1] = Point( .5f, -.5f);
		t.m_points[2] = Point( .5f,  .5f);
		pSquare->m_triangles.push_back(t);

		return pSquare;
	}


	Point m_movementVector;
	vector<Entity *> m_entities;
};


// -----------------------------------------------------------------------------


GameLoop* GameLoop::CreateInstance()
{
	return new GameLoopImpl();
}
