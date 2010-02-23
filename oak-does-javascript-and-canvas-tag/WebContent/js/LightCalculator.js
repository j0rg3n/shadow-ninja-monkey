

/**
 * The light calculator calculates a LightLayer.
 * 
 * @return
 */
function LightCalculator()
{
}

/**
 * Create a LightPolygon from the given light and scene
 * @return LightPolygon
 */
LightCalculator.prototype.calculateLightPolygon = function(light, scene)
{
	var visibleEdges = scene.visibleEdges(light);
	var sorter = new EdgeSorter();
	var sortedEdges = sorter.sortEdgePoints(light, visibleEdges);
	var edgeStack = new EdgeStack(light);

	// Scan through once (to build a starting stack)
	for(var i = 0; i < sortedEdges.length; i++)
	{
		edgeStack.addEdgePoint(sortedEdges[i]);
	}
	
	// Scan through again to create the light-polygon
	var lightPolygon = new LightPolygon();
	for(var i = 0; i < sortedEdges.length; i++)
	{
		var polyPoints = edgeStack.addEdgePoint(sortedEdges[i]);
		for(var j = 0; j < polyPoints.length; j++)
		{
			lightPolygon.addPoint(polyPoints[j]);
		}
	}
	
	return lightPolygon;
};


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
		
		equals( polyPoints.length, 6, "6 poly-points expected");
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