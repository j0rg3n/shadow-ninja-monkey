
function LightLayer()
{
	this.polygon = [];
}

/**
 * Add a point to the light-polygon.
 * 
 * @param Point p
 * @return
 */
LightLayer.prototype.addPolygonPoint = function(p)
{
	this.polygon.push(p);
};

/**
 * Get the polygon
 * 
 * @return Point[] - The array of points that describe the light polygon.
 */
LightLayer.prototype.getPolygon = function()
{
	return this.polygon;
};



LightLayer.unitTests = function()
{
	module('LightLayer');

	test("addPolygonPoint", function() {
		var lightLayer = new LightLayer();
		lightLayer.addPolygonPoint(new Point(42, 43));
		lightLayer.addPolygonPoint(new Point(14, -56));
		var poly = lightLayer.getPolygon();
		equals( poly.length, 2, "Two points in the polygon");
		equals( poly[0].x, 42, "x-value of point[0]");
		equals( poly[0].y, 43, "y-value of point[0]");
		equals( poly[1].x, 14, "x-value of point[1]");
		equals( poly[1].y, -56, "y-value of point[1]");
	});
};