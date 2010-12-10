#ifndef _POINT_H_INCLUDED
#define _POINT_H_INCLUDED


#include <stdlib.h>
#include <limits.h>
#include <math.h>


// -------------------------------------------------------------------------


class Edge;


// -------------------------------------------------------------------------


class Point 
{
public:
	Point() : x(0), y(0), m_nEdgeOffset(NO_EDGE)
	{
	}

	
	Point(float x, float y) : m_nEdgeOffset(NO_EDGE)
	{
		this->x = x;
		this->y = y;	
	}
	
	
	Point(const Point& other) : x(other.x), y(other.y), m_nEdgeOffset(NO_EDGE)
	{
	}
	
	
	/**
	 * Is this point left of the line A->B
	 */
	bool IsLeftOf(const Point& A, const Point& B) const
	{
		return IsLeftOfRaw(A, B) > 0;
	}
	
	
	float IsLeftOfRaw(const Point& A, const Point& B) const
	{
		return ((B.x-A.x)*(y-A.y) - (B.y-A.y)*(x-A.x) );
	}
	
	
	/**
	 * @param Point p
	 * @return Point
	 */
	Point Subtract(const Point& p) const
	{
		return Point(x-p.x, y-p.y);
	}
	
	
	/**
	 * @param float x
	 * @param Point p1
	 * @param Point p2
	 * @return Point 
	 */
	Point &xIntersectionLocal(float x, const Point& p1, const Point& p2)
	{
		this->x = x;
		Point dir = p2.Subtract(p1);
		float dist = (x - p2.x);
		this->y = p2.y + (dir.y/dir.x) * dist;
		return *this;
	}
	
	
	/**
	 * 
	 * @param float y
	 * @param Point p1
	 * @param Point p2
	 * @return Point 
	 */
	Point &YIntersectionLocal(float y, const Point& p1, const Point& p2)
	{
		this->y = y;
		Point dir = p2.Subtract(p1);
		float dist = (y - p2.y);
		this->x = p2.x + (dir.x/dir.y) * dist;
		return *this;
	}
	
	
	/**
	 * @return Point
	 */
	Point &NormalizeLocal()
	{
		float l = sqrt(x*x + y*y);
		x = x / l;
		y = y / l;
		return *this;
	}
	
	
	/**
	 * @param float s
	 * @return Point
	 */
	Point &MultiplyLocal(float s)
	{
		x = x*s;
		y = y*s;
		return *this;
	}
	
	
	/**
	 * @param Point p 
	 * @return float
	 */
	float DistanceToSquared(const Point &p) const
	{
		float dx = x-p.x;
		float dy = y-p.y;
		return dx*dx + dy*dy;
	}
	
	
	const Edge* GetEdge() const
	{
		if (m_nEdgeOffset == NO_EDGE)
		{
			return NULL;
		}
		
		return reinterpret_cast<const Edge *> (reinterpret_cast<const char *> (this) + m_nEdgeOffset);
	}
	
	
	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}
	
	
public:
	// TODO: Protect with read-only accessors.
	float x;
	float y;
	
	
private:
	friend class Edge;
	
	
	Point(const Point& other, const Edge& edge) : x(other.x), y(other.y), m_nEdgeOffset(GetEdgeOffset(*this, edge))
	{
	}
	
	
	static int GetEdgeOffset(const Point& point, const Edge& edge)
	{
		return reinterpret_cast<const char *> (&point) - reinterpret_cast<const char *> (&edge);
	}
	
	
	//!\brief Want to use sizeof(Edge), but that's not available.
	static const int NO_EDGE = INT_MAX;
	
	//!\brief Relative offset to edge, or NO_EDGE for detached points.
	int m_nEdgeOffset;
	
	/*
	 Point.unitTests()
	 {
	 module("Point");
	 
	 test("leftOf tests", function() {
	 var point = new Point(10, 10);
	 var origo = new Point(0, 0);
	 var xaxis = new Point(10, 0);
	 var yaxis = new Point(0, 10);
	 ok( point.leftOf(origo, xaxis), "Point is left of xaxis");
	 ok( !point.leftOf(xaxis, origo), "Point is right of reversed xaxis");
	 ok( !point.leftOf(origo, yaxis), "Point is right of yaxis");
	 ok( point.leftOf(yaxis, origo), "Point is left of reversed yaxis");
	 });
	 
	 test("subtract", function() {
	 var point = new Point(2, 7.5);
	 var p1 = new Point(1, 2);
	 
	 equals(point.subtract(p1).x, 1, "X value");
	 equals(point.subtract(p1).y, 5.5, "Y value");
	 });
	 
	 test("distanceToSquared", function() {
	 var p1 = new Point(1, 1);
	 var p2 = new Point(31, 41);
	 
	 equals(p1.distanceToSquared(p2), 2500, "Squared-distance");
	 });
	 
	 test("normalizeLocal", function() {
	 var point = new Point(3, 4).normalizeLocal();
	 equals(point.x, 3/5, "X value");
	 equals(point.y, 4/5, "Y value");
	 
	 point = new Point(3, -4).normalizeLocal();
	 equals(point.x, 3/5, "X value");
	 equals(point.y, -4/5, "Y value");
	 });
	 
	 
	 test("xIntersectionLocal", function() {
	 var point = new Point(0,0);
	 var p1 = new Point(1, 1);
	 var p2 = new Point(2, 2);
	 
	 point.xIntersectionLocal(0, p1, p2);
	 equals(point.y, 0, "Y value");
	 point.xIntersectionLocal(0, p2, p1);
	 equals(point.y, 0, "Y value");
	 
	 p1 = new Point(1, 1);
	 p2 = new Point(2, 0);
	 point.xIntersectionLocal(0, p1, p2); 
	 equals(point.y, 2, "Y value");
	 point.xIntersectionLocal(0, p2, p1); 
	 equals(point.y, 2, "Y value");
	 });
	 
	 test("yIntersectionLocal", function() {
	 var point = new Point(0,0);
	 var p1 = new Point(1, 1);
	 var p2 = new Point(2, 2);
	 
	 point.yIntersectionLocal(0, p1, p2);
	 equals(point.x, 0, "X value");
	 point.yIntersectionLocal(0, p2, p1);
	 equals(point.x, 0, "X value");
	 
	 p1 = new Point(1, 1);
	 p2 = new Point(0, 2);
	 point.yIntersectionLocal(0, p1, p2); 
	 equals(point.x, 2, "X value");
	 point.yIntersectionLocal(0, p2, p1); 
	 equals(point.x, 2, "X value");
	 });
	 };
	 */
};

#endif // _POINT_H_INCLUDED