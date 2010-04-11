
/**
 * Create an edge between two point, 
 * it is only visible from it's left side.
 * 
 * @param from
 * @param to
 * @return
 */
function Edge(from, to)
{
	this.from = from;
	this.to = to;
	
	// Link the points back to us
	this.from.edge = this;
	this.to.edge = this;
}

/**
 * Tell if this edge is 
 * @param Point light
 * @return
 */
Edge.prototype.isVisible = function(light)
{
	return light.leftOf(this.from, this.to);
};


Edge.prototype.nearestPoint = function(fromPoint)
{
	var u = ((fromPoint.x - this.from.x)*(this.to.x - this.from.x) + (fromPoint.y - this.from.y)*(this.to.y - this.from.y)) /
		(this.lengthSquared());
	
	// Clamp to [0,1]
	u = Math.max(0, Math.min(1, u));
	return new Point(
		this.from.x + u * (this.to.x - this.from.x), 
		this.from.y + u * (this.to.y - this.from.y)
	);
};

Edge.prototype.lengthSquared = function()
{
	var dx = this.to.x - this.from.x;
	var dy = this.to.y - this.from.y;
	return dx*dx + dy*dy;
};

Edge.prototype.length = function()
{
	return Math.sqrt(this.lengthSquared());
};

Edge.prototype.intersection = function(p3, p4)
{
	var p1 = this.from;
	var p2 = this.to;
	var numerator = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
	var denominator = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
	if(denominator == 0)
		return null; // They are parallel
	var ua = numerator/denominator;
	if(ua < 0 || ua > 1)
		return null; // The crossing does not hit the edge
	//var ub = ((p2.x - p1.x)*(p1.y - p3.y) - (p4.y - p1.y)*(p1.x - p3.x))/denominator;
	
	return new Point(
			p1.x + ua*(p2.x - p1.x),
			p1.y + ua*(p2.y - p1.y)
	);
};

Edge.unitTests = function()
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