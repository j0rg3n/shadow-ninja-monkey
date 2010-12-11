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
	 * Create a new scene with the given width and height.
	 * 
	 * @param width
	 * @param height
	 * @return
	 */
	Scene(float width, float height)
	{
		this->width = width;
		this->height = height;
	}
	
	/**
	 * Create an inverted rectangle used to "catch" all light.
	 * 
	 * @return
	 */
	void AddLightBoundingBox()
	{
		// Add an inverse box around the scene to avoid light getting 'out'
		this->AddEdge(Point(0,            0+this->height), Point(0,            0));
		this->AddEdge(Point(0+this->width, 0+this->height), Point(0,            0+this->height));
		this->AddEdge(Point(0+this->width, 0),             Point(0+this->width, 0+this->height));
		this->AddEdge(Point(0,            0),             Point(0+this->width, 0));
	};
	
	/**
	 * Add a new edge between p1 and p2.
	 * @param Point p1
	 * @param Point p2
	 * @return
	 */
	void AddEdge(const Point& p1, const Point& p2)
	{
		this->edges.push_back(Edge(p1, p2));
	};
	
	/**
	 * Create a closed rectangle.
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 * @return
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
	
private:
	float width;
	float height;
	std::vector<Edge> edges;
	std::vector<Point> lights;
	
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
