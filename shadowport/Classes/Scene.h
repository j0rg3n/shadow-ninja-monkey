#ifndef _SCENE_H_INCLUDED
#define _SCENE_H_INCLUDED


#include <vector>
#include "Edge.h"


namespace snm
{


class Scene
{
public:
	/**
	 * Add a new edge between p1 and p2.
	 */
	void AddEdge(const Point& p1, const Point& p2)
	{
		this->edges.push_back(Edge(p1, p2));
	};
	
	/**
	 * Create an inverted rectangle used to "catch" all light.
	 * 
	 * NOTE: A valid scene must be contained in a closed, inside-out shape.
	 */
	void AddInverseRectangle(float x, float y, float w, float h)
	{
		this->AddEdge(Point(x,   y),   Point(x+w, y));
		this->AddEdge(Point(x+w, y),   Point(x+w, y+h));
		this->AddEdge(Point(x+w, y+h), Point(x,   y+h));
		this->AddEdge(Point(x,   y+h), Point(x,   y));
	};
	
	/**
	 * Create a closed rectangle.
	 */
	void AddRectangle(float x, float y, float w, float h)
	{
		this->AddEdge(Point(x,   y),   Point(x,   y+h));
		this->AddEdge(Point(x,   y+h), Point(x+w, y+h));
		this->AddEdge(Point(x+w, y+h), Point(x+w, y));
		this->AddEdge(Point(x+w, y),   Point(x,   y));
	};
	
	/**
	 * Return an array of visible edges.
	 * 
	 * @param Point light - the position of the light source.
	 * @return
	 */
	void VisibleEdges(const Point& light, std::vector<Edge>& result) const
	{
		for(size_t i = 0; i < this->edges.size(); i++)
		{
			const Edge& e = this->edges[i];
			// Why does oak check the type of the entry?
			if(/*e instanceof Edge && */e.IsVisible(light))
			{
				result.push_back(e);
			}
		}
	};
	
	const std::vector<Edge>& GetEdges() const
	{
		return edges;
	}
	
private:
	std::vector<Edge> edges;
	
	/*
	 Scene.unitTests()
	 {
	 module('Scene');
	 
	 test("AddEdge", function() {
	 var scene = new Scene(100, 100);
	 var from = Point(9, 9);
	 var to = Point(11, 9);
	 
	 scene.AddEdge(from, to);
	 equals( scene.edges.length, 1, "Edge count");
	 equals( scene.edges[0].from, from, "From point");
	 equals( scene.edges[0].to, to, "To point");
	 });
	 
	 test("visibleEdges (visible)", function(){
	 var scene = new Scene(100, 100);
	 scene.AddEdge(Point(1, 1), Point(3, 1));
	 
	 var edges = scene.visibleEdges(Point(2, 2));
	 equals(edges.length, 1, 'Visible edge accepted');
	 });
	 test("visibleEdges (not visible)", function(){
	 var scene = new Scene(100, 100);
	 scene.AddEdge(Point(1, 3), Point(3, 3));
	 
	 var edges = scene.visibleEdges(Point(2, 2));
	 equals(edges.length, 0, 'Invisible edge declined');
	 });
	 
	 test("addLightBoundingBox", function(){
	 var scene = new Scene(200, 300);
	 scene.addLightBoundingBox();
	 
	 equals(scene.edges.length, 4, '4 edges');
	 });
	 };
	 */
};

}; // namespace snm	

#endif // _SCENE_H_INCLUDED
