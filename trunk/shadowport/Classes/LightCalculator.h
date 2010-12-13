#ifndef _LIGHTCALCULATOR_H_INCLUDED
#define _LIGHTCALCULATOR_H_INCLUDED


#include <vector>
#include "Scene.h"
#include "EdgeSorter.h"
#include "EdgeStack.h"
#include "LightPolygon.h"


namespace snm
{
	

/**
 * The light calculator calculates a LightLayer.
 * 
 * @return
 */
class LightCalculator
{
public:
	/**
	 * Create a LightPolygon from the given light and scene
	 * @return LightPolygon
	 */
	static void CalculateLightPolygon(const Point& light, const Scene& scene, LightPolygon& lightPolygon)
	{
		std::vector<Edge> visibleEdges;
		scene.VisibleEdges(light, visibleEdges);
		
		EdgeSorter sorter;
		std::vector<EdgePointReference> sortedEdges;
		std::vector<Edge> nearestEdgeParts;
		sorter.SortEdgePoints(light, visibleEdges, nearestEdgeParts, sortedEdges);
		
		EdgeStack edgeStack(light);

		// Allocate this temp buffer here to avoid reallocation for every edge.
		std::vector<Point> lightPolyPoints;

		// Scan through once (to build a starting stack)
		for(size_t i = 0; i < sortedEdges.size(); i++)
		{
			// TODO: Avoid adding unused points to the vector.
			lightPolyPoints.clear();
			edgeStack.AddEdgePoint(sortedEdges[i], lightPolyPoints);
		}
		
		// Scan through again to create the light-polygon
		for(size_t i = 0; i < sortedEdges.size(); i++)
		{
			// TODO: Remove 'polyPoints' by making AddEdgePoint add directly to the light polygon.
			lightPolyPoints.clear();
			edgeStack.AddEdgePoint(sortedEdges[i], lightPolyPoints);
			for(size_t j = 0; j < lightPolyPoints.size(); j++)
			{
				lightPolygon.AddPoint(lightPolyPoints[j]);
			}
		}
	};
	
	/*
	 LightCalculator.unitTests = function()
	 {
	 module('LightCalculator');
	 
	 test("calculateLightPolygon with light bounding box", function() {
	 var light = new Point(3, 2);
	 var scene = new Scene(4, 4);
	 scene.addLightBoundingBox();
	 var lightCalculator = new LightCalculator();
	 var lightPolygon = lightCalculator.calculateLightPolygon(light, scene);
	 
	 var polyPoints = lightPolygon.getPoints();
	 
	 equals( polyPoints.size(), 6, "6 poly-points expected");
	 equals( polyPoints[0].x, 4, "Start Cross-point (x)");
	 equals( polyPoints[0].y, 2, "Start Cross-point (y)");
	 equals( polyPoints[1].x, 4, "Upper-Right corner (x)");
	 equals( polyPoints[1].y, 4, "Upper-Right corner (y)");
	 equals( polyPoints[2].x, 0, "Upper-Left corner (x)");
	 equals( polyPoints[2].y, 4, "Upper-Left corner (y)");
	 equals( polyPoints[3].x, 0, "Lower-Left corner (x)");
	 equals( polyPoints[3].y, 0, "Lower-Left corner (y)");
	 equals( polyPoints[4].x, 4, "Lower-Right corner (x)");
	 equals( polyPoints[4].y, 0, "Lower-Right corner (y)");
	 equals( polyPoints[5].x, 4, "End Cross-point (x)");
	 equals( polyPoints[5].y, 2, "End Cross-point (y)");
	 });
	 
	 test("calculateLightPolygon with light bounding box and a rectangle", function() {
	 var light = new Point(50, 50);
	 var scene = new Scene(200, 200);
	 scene.addLightBoundingBox();
	 scene.addRectangle(10, 10, 20, 20);
	 var lightCalculator = new LightCalculator();
	 var lightPolygon = lightCalculator.calculateLightPolygon(light, scene);
	 
	 var polyPoints = lightPolygon.getPoints();
	 });
	 
	 };
	 */	
};

}; // namespace snm	

#endif // _LIGHTCALCULATOR_H_INCLUDED