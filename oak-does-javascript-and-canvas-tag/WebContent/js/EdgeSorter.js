
function EdgeSorter()
{
}

/**
 * Find the edge nearest the light.
 * 
 * @param Point light
 * @param Edge[] edges
 * @return
 */
EdgeSorter.prototype.findNearestEdge = function(light,edges)
{
	var result = null;
	for(var i = 0; i < edges.length; i++)
	{
		var e = edges[i];
		if(result == null || e.nearestPoint(light).distanceToSquared(light) < result.nearestPoint(light).distanceToSquared(light))
		{
			result = e;
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
EdgeSorter.prototype.sortEdgePoints = function(light,edges)
{
	var nearestEdge = this.findNearestEdge(light, edges);
	var nearestPoint = nearestEdge.nearestPoint(light);
	var nearestEdgePart2 = new Edge(new Point(nearestEdge.from.x, nearestEdge.from.y), new Point(nearestPoint.x, nearestPoint.y));
	var nearestEdgePart1 = new Edge(new Point(nearestPoint.x, nearestPoint.y), new Point(nearestEdge.to.x, nearestEdge.to.y));
	var edgePoints = [];
	if(nearestEdgePart1.lengthSquared() > 0)
	{
		edgePoints.push(nearestEdgePart1.to);
	}
	if(nearestEdgePart2.lengthSquared() > 0)
	{
		edgePoints.push(nearestEdgePart2.from);
	}
	for(var i = 0; i < edges.length; i++)
	{
		var edge = edges[i];
		if(edge != nearestEdge)
		{
			edgePoints.push(edge.from, edge.to);
		}
	}
	// Sort these counter-clockwise, with  (light -> nearestPoint)
	edgePoints.sort(function(p1, p2){
		var p1LeftOfStart = p1.leftOf(light, nearestPoint);
		var p2LeftOfStart = p2.leftOf(light, nearestPoint);
		if(p1LeftOfStart != p2LeftOfStart)
			return (p2LeftOfStart - p1LeftOfStart);
		return p1.leftOfRaw(light, p2);
	});
	
	
	// Insert the divided edge at the start and at the end.
	if(nearestEdgePart1.lengthSquared() > 0)
	{
		edgePoints.unshift(nearestEdgePart1.from);
	}
	if(nearestEdgePart2.lengthSquared() > 0)
	{
		edgePoints.push(nearestEdgePart2.to);
	}
	
	return edgePoints;
};

EdgeSorter.unitTests = function()
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