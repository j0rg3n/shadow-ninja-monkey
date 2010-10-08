#pragma once
#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED


// TODO: Move Point.h to public includes.
#include "../../lib/math/Point.h"


// -----------------------------------------------------------------------------


class Triangle
{
public:
	Point m_points[3];
};


// -----------------------------------------------------------------------------


class Geometry
{
public:
	typedef std::vector<Triangle> TriangleList;

	TriangleList m_triangles;
};


// -----------------------------------------------------------------------------


class Entity
{
public:
	Entity();
	Entity(Geometry* pGeometry, const Point& pos);
	~Entity();

	//void AddTask(CallQueue::DispatchEntry task);
	//!\brief Render one frame.
	//void Run();

	Geometry* m_pGeometry;
	Point m_pos;
	std::string m_sName;
};


#endif // ENTITY_H_INCLUDED