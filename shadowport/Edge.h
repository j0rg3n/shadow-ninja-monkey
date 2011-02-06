#ifndef _EDGE_H_INCLUDED
#define _EDGE_H_INCLUDED


#include "MathExtra.h"
#include "Point.h"


namespace snm
{


// -----------------------------------------------------------------------------


class Edge
{
public:
	Edge()
	{
	}
	
	/**
	 * Create an edge between two point - also known as a line segment.
	 *   
	 * An edge is only visible from it's left side.
	 * 
	 * @param from
	 * @param to
	 * @return
	 */
	Edge(const Point& from, const Point& to) : from(from), to(to)
	{	
	}
	
	
	Edge(const Edge& other) : from(other.from), to(other.to)
	{
	}
	
	
	/**
	 * Tell if this edge is 
	 * @param Point light
	 * @return
	 */
	bool IsVisible(const Point& light) const
	{
		return light.IsLeftOf(this->from, this->to);
	}
	
	
	/**
	 * Get the nearest point on the edge, it might be the end-points. 
	 * @param Point fromPoint
	 * @return
	 */
	Point NearestPoint(const Point& fromPoint) const
	{
		float u = ((fromPoint.x - this->from.x)*(this->to.x - this->from.x) + (fromPoint.y - this->from.y)*(this->to.y - this->from.y)) /
		(this->LengthSquared());
		
		// Clamp to [0,1]
		u = max(0, min(1, u));
		return Point(
					 this->from.x + u * (this->to.x - this->from.x), 
					 this->from.y + u * (this->to.y - this->from.y)
					 );
	}
	
	
	/**
	 * Get the distance to the given point. 
	 * @param Point fromPoint
	 * @return
	 */
	float DistanceToPointSquared(const Point& fromPoint) const
	{
		float u = ((fromPoint.x - this->from.x)*(this->to.x - this->from.x) + (fromPoint.y - this->from.y)*(this->to.y - this->from.y)) /
		(this->LengthSquared());
		
		// Clamp to [0,1]
		u = max(0, min(1, u));
		float dx = (this->from.x + u * (this->to.x - this->from.x)) - fromPoint.x; 
		float dy = (this->from.y + u * (this->to.y - this->from.y)) - fromPoint.y;
		
		return dx*dx + dy*dy;
	}
	
	
	/**
	 * Calculate the square of the length of the edge.
	 * 
	 * @return float
	 */
	float LengthSquared() const
	{
		float dx = this->to.x - this->from.x;
		float dy = this->to.y - this->from.y;
		return dx*dx + dy*dy;
	}
	
	
	/**
	 * Calculate the length of the edge.
	 * 
	 * @return float
	 */
	float Length() const
	{
		return sqrt(this->LengthSquared());
	}
	
	
	/**
	 * Calculate the intersection between the edge and the edge defined by the two points.
	 * 
	 * If the lines are parallel or the intersection occurs "outside" this edge the method returns null. 
	 * 
	 * @param Point p3
	 * @param Point p4
	 * @return Point
	 */
	bool Intersection(const Point& p3, const Point& p4, Point& result) const
	{
		const Point& p1 = this->from;
		const Point& p2 = this->to;
		float numerator = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
		float denominator = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
		if(denominator == 0)
			return false; // They are parallel
		
		float ua = numerator/denominator;
		if(ua < 0 || ua > 1)
			return false; // The crossing does not hit the edge
		//float ub = ((p2.x - p1.x)*(p1.y - p3.y) - (p4.y - p1.y)*(p1.x - p3.x))/denominator;
		
		result = Point(
					   p1.x + ua*(p2.x - p1.x),
					   p1.y + ua*(p2.y - p1.y)
					   );
		return true;
	}
	
	
	bool operator==(const Edge& other) const
	{
		return from == other.from && to == other.to;
	}
	
	
public:
	Point from;
	Point to;
	
	/*
	 Edge.unitTests()
	 {
	 module('Edge');
	 
	 test("isVisible", function() {
	 var light = new Point(10, 10);
	 var e1 = new Edge(new Point(9, 9), new Point(11, 9));
	 var e2 = new Edge(new Point(9, 9), new Point(9, 11));
	 
	 ok( e1.isVisible(light), "Edge is visible");
	 ok( !e2.isVisible(light), "Edge is not visible");
	 });
	 
	 test("setParent", function() {
	 var from = new Point(9, 9);
	 var to = new Point(11, 9);
	 var edge = new Edge(from, to);
	 
	 equals( from.edge, edge, "From knows it's edge");
	 equals( to.edge, edge, "To knows it's edge");
	 });
	 
	 test("lengthSquared", function() {
	 var from = new Point(1, 1);
	 var to = new Point(4, 5);
	 var edge = new Edge(from, to);
	 
	 equals( edge.lengthSquared(), 3*3+4*4, "Square-length of edge");
	 });
	 
	 test("length", function() {
	 var from = new Point(1, 1);
	 var to = new Point(4, 5);
	 var edge = new Edge(from, to);
	 
	 equals( edge.length(), 5, "Length of edge");
	 });
	 
	 test("nearestPoint", function() {
	 var from = new Point(1, 1);
	 var to = new Point(5, 1);
	 var edge = new Edge(from, to);
	 
	 var p1 = edge.nearestPoint(new Point(3, 2));
	 equals( p1.x, 3, "x-value");
	 equals( p1.y, 1, "y-value");
	 
	 var p2 = edge.nearestPoint(new Point(0, 0));
	 equals( p2.x, 1, "x-value");
	 equals( p2.y, 1, "y-value");
	 });
	 
	 test("distanceToPointSquared", function() {
	 var from = new Point(1, 1);
	 var to = new Point(5, 1);
	 var edge = new Edge(from, to);
	 
	 var d1 = edge.distanceToPointSquared(new Point(3, 2));
	 equals( d1, 1, "distance: (1+0)");
	 
	 var d2 = edge.distanceToPointSquared(new Point(0, 0));
	 equals( d2, 1+1, "distance: (1+1)");
	 });
	 
	 test("intersection", function(){
	 var from = new Point(1, 1);
	 var to = new Point(5, 1);
	 var edge = new Edge(from, to);
	 
	 var i1 = edge.intersection(new Point(2, 5), new Point(2, 4));
	 equals( i1.x, 2, "x-value");
	 equals( i1.y, 1, "y-value");
	 
	 var i2 = edge.intersection(new Point(2, 3), new Point(3, 2));
	 equals( i2.x, 4, "x-value");
	 equals( i2.y, 1, "y-value");
	 
	 var i3 = edge.intersection(new Point(2, 2), new Point(3, 2));
	 equals( i3, null, "lines are parallel");
	 
	 var i4 = edge.intersection(new Point(6, 3), new Point(6, 2));
	 equals( i4, null, "lines cross outside edge");
	 });
	 
	 };
	 */
};

}; // namespace snm	

#endif // _EDGE_H_INCLUDED