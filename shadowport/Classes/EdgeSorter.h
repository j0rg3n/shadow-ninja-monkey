#ifndef _EDGESORTER_H_INCLUDED
#define _EDGESORTER_H_INCLUDED


#include <algorithm>
#include <vector>
#include <cassert>
#include "Edge.h"


// -----------------------------------------------------------------------------


class EdgeSorter
{
	/**
	 * Find the edge nearest the light.
	 * 
	 * @param Point light
	 * @param Edge[] edges
	 * @return
	 */
	const Edge* FindNearestEdge(const Point& light, std::vector<Edge>& edges)
	{
		const Edge* result = NULL;
		for(size_t i = 0; i < edges.size(); i++)
		{
			const Edge& e = edges[i];
			if(result == NULL || e.NearestPoint(light).DistanceToSquared(light) < result->NearestPoint(light).DistanceToSquared(light))
			{
				result = &e;
			}
		}
		return result;
	};
	
	
	/**
	 * This method will return a sorted (counter-clockwise) list of edges (end/start-points) used for light-calculation.
	 * 
	 * @param Point light - The light to sort around
	 * @param Edge[] edges - An array of visible edges.
	 * @return
	 */
	void SortEdgePoints(const Point& light, std::vector<Edge>& edges, std::vector<Point>& edgePoints)
	{
		const Edge* nearestEdge = this->FindNearestEdge(light, edges);
		assert(nearestEdge != NULL);
		Point nearestPoint = nearestEdge->NearestPoint(light);
		Edge nearestEdgePart2 = Edge(Point(nearestEdge->from.x, nearestEdge->from.y), Point(nearestPoint.x, nearestPoint.y));
		Edge nearestEdgePart1 = Edge(Point(nearestPoint.x, nearestPoint.y), Point(nearestEdge->to.x, nearestEdge->to.y));
		
		if(nearestEdgePart1.LengthSquared() > 0)
		{
			edgePoints.push_back(nearestEdgePart1.to);
		}
		if(nearestEdgePart2.LengthSquared() > 0)
		{
			edgePoints.push_back(nearestEdgePart2.from);
		}
		for(size_t i = 0; i < edges.size(); i++)
		{
			const Edge& edge = edges[i];
			if(&edge != nearestEdge)
			{
				edgePoints.push_back(edge.from);
				edgePoints.push_back(edge.to);
			}
		}
		// Sort these counter-clockwise, with  (light -> nearestPoint)
		ComparePointsCCW comparison(light, nearestPoint);
		std::sort(edgePoints.begin(), edgePoints.end(), comparison);
		
		
		// Insert the divided edge at the start and at the end.
		if(nearestEdgePart1.LengthSquared() > 0)
		{
			edgePoints.insert(edgePoints.begin(), nearestEdgePart1.from);
		}
		if(nearestEdgePart2.LengthSquared() > 0)
		{
			edgePoints.push_back(nearestEdgePart2.to);
		}
	};

	
private:
	struct ComparePointsCCW
	{
	public:
		ComparePointsCCW(const Point& light, const Point& nearestPoint) : light(light), nearestPoint(nearestPoint)
		{
		}
		
		
		bool operator()(const Point& p1, const Point& p2) const
		{
			return Compare(p1, p2) < 0;
		}

		
	private:
		int Compare(const Point& p1, const Point& p2) const
		{
			bool p1LeftOfStart = p1.IsLeftOf(light, nearestPoint);
			bool p2LeftOfStart = p2.IsLeftOf(light, nearestPoint);
			if(p1LeftOfStart != p2LeftOfStart)
				return (p2LeftOfStart - p1LeftOfStart);
			return p1.IsLeftOfRaw(light, p2);
		}
		
		
	private:
		const Point& light;
		const Point& nearestPoint;
	};
	
	
	/*
	 EdgeSorter.unitTests()
	 {
	 module('EdgeSorter');
	 
	 test("findNearestEdge", function() {
	 var e0 = new Edge(new Point(1, 10), new Point(3, 10));
	 var e1 = new Edge(new Point(1, 1), new Point(3, 1));
	 var e2 = new Edge(new Point(3, 4), new Point(1, 4));
	 
	 var sorter = new EdgeSorter();
	 var nearest = sorter.findNearestEdge(new Point(2, 2), [e0, e1, e2]);
	 
	 equals( nearest, e1, "e1 is nearest");
	 });
	 
	 test("sortEdgePoints(simple)", function() {
	 var e1 = new Edge(new Point(1, 1), new Point(3, 1));
	 var sorter = new EdgeSorter();
	 var edgePoints = sorter.sortEdgePoints(new Point(2, 2), [e1]);
	 
	 equals(edgePoints.length, 4, "The nearest edge is divided into two, each with a from and two");
	 equals(edgePoints[0].x, 2, "x value of point 1/4");
	 equals(edgePoints[0].y, 1, "y value of point 1/4");
	 equals(edgePoints[1].x, 3, "x value of point 2");
	 equals(edgePoints[1].y, 1, "y value of point 2");
	 equals(edgePoints[2].x, 1, "x value of point 3");
	 equals(edgePoints[2].y, 1, "y value of point 3");
	 equals(edgePoints[3].x, 2, "x value of point 4/1");
	 equals(edgePoints[3].y, 1, "y value of point 4/1");
	 });
	 
	 test("sortEdgePoints (two edges)", function() {
	 var e1 = new Edge(new Point(1, 1), new Point(3, 1));
	 var e2 = new Edge(new Point(4, 4), new Point(0, 4));
	 var sorter = new EdgeSorter();
	 var edgePoints = sorter.sortEdgePoints(new Point(2, 2), [e1, e2]);
	 
	 equals(edgePoints.length, 6, "The nearest edge is divided into two, each with a from and two");
	 equals(edgePoints[0].x, 2, "x value of point 1/6");
	 equals(edgePoints[0].y, 1, "y value of point 1/6");
	 equals(edgePoints[1].x, 3, "x value of e1.to");
	 equals(edgePoints[1].y, 1, "y value of e1.to");
	 
	 equals(edgePoints[2].x, 4, "x value of e2.from");
	 equals(edgePoints[2].y, 4, "y value of e2.from");
	 equals(edgePoints[3].x, 0, "x value of e2.to");
	 equals(edgePoints[3].y, 4, "y value of e2.to");
	 
	 equals(edgePoints[4].x, 1, "x value of e1.from");
	 equals(edgePoints[4].y, 1, "y value of e1.from");
	 equals(edgePoints[5].x, 2, "x value of point 6/1");
	 equals(edgePoints[5].y, 1, "y value of point 6/1");
	 });
	 
	 test("sortEdgePoints(e1.from is nearest point)", function() {
	 var e1 = new Edge(new Point(1, 1), new Point(3, 1));
	 var sorter = new EdgeSorter();
	 var edgePoints = sorter.sortEdgePoints(new Point(0, 2), [e1]);
	 
	 equals(edgePoints.length, 2, "The nearest edge is not divided into two");
	 equals(edgePoints[0].x, 1, "x value of e1.from.x");
	 equals(edgePoints[0].y, 1, "y value of e1.from.y");
	 equals(edgePoints[1].x, 3, "x value of e1.to.x");
	 equals(edgePoints[1].y, 1, "y value of e1.to.y");
	 });
	 
	 test("sortEdgePoints(e1.to is nearest point)", function() {
	 var e1 = new Edge(new Point(-3, 1), new Point(-1, 1));
	 var sorter = new EdgeSorter();
	 var edgePoints = sorter.sortEdgePoints(new Point(0, 2), [e1]);
	 
	 equals(edgePoints.length, 2, "The nearest edge is not divided into two");
	 equals(edgePoints[0].x, -3, "x value of e1.from.x");
	 equals(edgePoints[0].y, 1, "y value of e1.from.y");
	 equals(edgePoints[1].x, -1, "x value of e1.to.x");
	 equals(edgePoints[1].y, 1, "y value of e1.to.y");
	 });
	 };
	 */
};

#endif // _EDGESORTER_H_INCLUDED