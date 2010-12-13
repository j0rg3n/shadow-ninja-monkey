#ifndef _POINTWITHEDGE_H_INCLUDED
#define _POINTWITHEDGE_H_INCLUDED


#include "Edge.h"


namespace snm
{

	
class Point;
	

class EdgePointReference 
{
public:
	operator const Point&() const
	{
		return *point;
	}
	
	
	const Point& GetPoint() const
	{
		return *point;
	}
	
	
	const Edge& GetEdge() const
	{
		return *edge;
	}
	
	
	bool IsFrom() const
	{
		return point == &edge->from;
	}
	
	
	bool IsTo() const
	{
		return point == &edge->to;
	}
	
		
private:
	EdgePointReference(const Point& point, const Edge& edge) : point(&point), edge(&edge)
	{
	}
	
	
	const Point* point;
	const Edge* edge;
	
	
	friend EdgePointReference FromOf(const Edge& edge);
	friend EdgePointReference ToOf(const Edge& edge);
};
	
	
inline EdgePointReference FromOf(const Edge& edge)
{
	return EdgePointReference(edge.from, edge);
}
	
	
inline EdgePointReference ToOf(const Edge& edge)
{
	return EdgePointReference(edge.to, edge);
}
	
	
};


#endif // _POINTWITHEDGE_H_INCLUDED