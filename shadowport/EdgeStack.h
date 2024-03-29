#ifndef _EDGESTACK_H_INCLUDED
#define _EDGESTACK_H_INCLUDED


#include <vector>
#include <cassert>
#include "EdgePointReference.h"
#include "Point.h"


namespace snm
{


class EdgeStack
{
public:
	/**
	 * A stack of edges sorted by their distance to the light-source.
	 * 
	 * @param Point light
	 * @return
	 */
	EdgeStack(const Point& light) : light(light)
	{
	}
	
	/**
	 * Add/Remove an edge by adding it's end-points.
	 * 
	 * @param Point p
	 * @return Point[] - array of points to be added to the light polygon.
	 */
	void AddEdgePoint(const EdgePointReference& p, std::vector<Point>& lightPolyPoints)
	{
		if(p.IsFrom())
		{
			for(size_t i = 0; i < this->edges.size(); i++)
			{
				Point intersect;
				bool bSuccess = this->edges[i].Intersection(this->light, p, intersect);
				//assert(bSuccess);
				
				if(bSuccess && p.GetPoint().DistanceToSquared(this->light) <= intersect.DistanceToSquared(this->light))
				{
					this->edges.insert(edges.begin() + i, p.GetEdge());
					if(i == 0)
					{
						// a point on the previous edge, and the new point
						lightPolyPoints.push_back(intersect);
						lightPolyPoints.push_back(p);
						return;
					}
					// We have been added behind an other edge
					return;
				}
			}
			this->edges.push_back(p.GetEdge());
			if(this->edges.size() == 1)
			{
				// We are the first edge to be added
				lightPolyPoints.push_back(p);
				return;
			}
			else
			{
				// We were added behind something else
				return;
			}
		}
		else // Remove and edge
		{
			for(size_t i = 0; i < this->edges.size(); i++)
			{
				if(this->edges[i] == p.GetEdge())
				{
					this->edges.erase(edges.begin() + i);
					if(i == 0) // Was it the front edge that ended?
					{
						if(this->edges.size() == 0)
						{
							// This was the last edge
							lightPolyPoints.push_back(p);
							return;
						}
						else
						{
							Point intersect;
							bool bSuccess = this->edges[0].Intersection(this->light, p, intersect);
							//assert(bSuccess);
							if (bSuccess)
							{
								lightPolyPoints.push_back(p);
								lightPolyPoints.push_back(intersect);
								return;
							}
						}
					}
					else
					{
						// It was not the front-edge that ended
						return;
					}
				}
			}
			
			// Since we could not find the edge, we assume it has not even been started.
			return;
		}
	};
	
public:
	std::vector<Edge> edges;
	const Point& light;
	
	/*
	 EdgeStack.unitTests()
	 {
	 module('EdgeStack');
	 
	 test("addEdgePoint add one edge", function() {
	 var stack = new EdgeStack(new Point(0,0));
	 var e1 = new Edge(new Point(2, 1), new Point(0, 1));
	 stack.addEdgePoint(e1.from);
	 
	 equals( stack.edges.length, 1, "There should be one edge");
	 equals( stack.edges[0], e1, "It should be e1");
	 });
	 
	 test("addEdgePoint add two edges", function() {
	 var stack = new EdgeStack(new Point(0,0));
	 var e1 = new Edge(new Point(2, 1), new Point(0, 1));
	 var e2 = new Edge(new Point(2, 2), new Point(0, 2));
	 stack.addEdgePoint(e1.from);
	 stack.addEdgePoint(e2.from);
	 
	 equals( stack.edges.length, 2, "There should be two edges");
	 equals( stack.edges[0], e1, "It should be e1");
	 equals( stack.edges[1], e2, "It should be e1");
	 });
	 
	 test("addEdgePoint remove e2", function() {
	 var stack = new EdgeStack(new Point(0,0));
	 var e1 = new Edge(new Point(2, 1), new Point(0, 1));
	 var e2 = new Edge(new Point(2, 2), new Point(0, 2));
	 stack.addEdgePoint(e1.from);
	 stack.addEdgePoint(e2.from);
	 stack.addEdgePoint(e2.to);
	 
	 equals( stack.edges.length, 1, "There should be one edge");
	 equals( stack.edges[0], e1, "It should be e1");
	 });
	 
	 test("addEdgePoint remove e1", function() {
	 var stack = new EdgeStack(new Point(0,0));
	 var e1 = new Edge(new Point(2, 1), new Point(0, 1));
	 var e2 = new Edge(new Point(2, 2), new Point(0, 2));
	 stack.addEdgePoint(e1.from);
	 stack.addEdgePoint(e2.from);
	 stack.addEdgePoint(e1.to);
	 
	 equals( stack.edges.length, 1, "There should be one edge");
	 equals( stack.edges[0], e2, "It should be e2");
	 });
	 
	 test("addEdgePoint simple edge returns from and to", function() {
	 var stack = new EdgeStack(new Point(0,0));
	 var e1 = new Edge(new Point(2, 1), new Point(0, 1));
	 var polyPoints1 = stack.addEdgePoint(e1.from);
	 
	 equals( polyPoints1.length, 1, "One poly-point");
	 equals( polyPoints1[0], e1.from, "It should be e1.from");
	 
	 var polyPoints2 = stack.addEdgePoint(e1.to);
	 equals( polyPoints2.length, 1, "One poly-point");
	 equals( polyPoints2[0], e1.to, "It should be e1.to");
	 });
	 
	 test("addEdgePoint behind edge yields empty result", function() {
	 var stack = new EdgeStack(new Point(1,0));
	 var e1 = new Edge(new Point(4, 1), new Point(-4, 1));
	 var e2 = new Edge(new Point(2, 2), new Point(0, 2));
	 stack.addEdgePoint(e1.from);
	 var polyPoints = stack.addEdgePoint(e2.from);
	 
	 equals( polyPoints.length, 0, "Empty result");
	 });
	 
	 test("addEdgePoint in front of edge returns two points", function() {
	 var stack = new EdgeStack(new Point(1,0));
	 var e1 = new Edge(new Point(4, 2), new Point(-4, 2));
	 var e2 = new Edge(new Point(1, 1), new Point(0, 1));
	 stack.addEdgePoint(e1.from);
	 var polyPoints = stack.addEdgePoint(e2.from);
	 
	 equals( polyPoints.length, 2, "Two points");
	 equals( polyPoints[0].x, 1, "Point on e1.x");
	 equals( polyPoints[0].y, 2, "Point on e1.y");
	 equals( polyPoints[1], e2.from, "e2.from");
	 });
	 
	 test("addEdgePoint remove an edge in front of an other edge returns two points", function() {
	 var stack = new EdgeStack(new Point(1,0));
	 var e1 = new Edge(new Point(4, 2), new Point(-4, 2));
	 var e2 = new Edge(new Point(2, 1), new Point(1, 1));
	 stack.addEdgePoint(e1.from);
	 stack.addEdgePoint(e2.from);
	 var polyPoints = stack.addEdgePoint(e2.to);
	 
	 equals( polyPoints.length, 2, "Two points");
	 equals( polyPoints[0], e2.to, "e2.to");
	 equals( polyPoints[1].x, 1, "Point on e1.x");
	 equals( polyPoints[1].y, 2, "Point on e1.y");
	 });
	 };
	 */
};

}; // namespace snm	

#endif // _EDGESTACK_H_INCLUDED