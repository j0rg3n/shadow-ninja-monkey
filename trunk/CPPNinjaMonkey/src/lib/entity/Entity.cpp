#include "entity/Entity.h"


// -----------------------------------------------------------------------------


Entity::Entity() : m_pGeometry(NULL)
{
}


Entity::Entity(Geometry* pGeometry, const Point& pos) : m_pGeometry(pGeometry), m_pos(pos)
{
}


Entity::~Entity()
{
	if (m_pGeometry)
	{
		delete m_pGeometry;
	}
}
