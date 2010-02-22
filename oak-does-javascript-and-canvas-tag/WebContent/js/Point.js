
function Point(x, y)
{
	this.x = x;
	this.y = y;	
	return this;
}
	
	/**
	 * Is this point left of the line A->B
	 */
Point.prototype.leftOf = function(A,B)
{
	return this.leftOfRaw(A,B) > 0;
};
	
Point.prototype.leftOfRaw = function(A,B)
{
	return ( (B.x-A.x)*(this.y-A.y) - (B.y-A.y)*(this.x-A.x) );
};

/**
 * @param Point p
 * @return Point
 */
Point.prototype.subtract = function(p)
{
	return new Point(this.x-p.x, this.y-p.y);
};

/**
 * @param float x
 * @param Point p1
 * @param Point p2
 * @return Point 
 */
Point.prototype.xIntersectionLocal = function(x, p1, p2)
{
	this.x = x;
	var dir = p2.subtract(p1);
	var dist = (x - p2.x);
	this.y = p2.y + (dir.y/dir.x) * dist;
	return this;
};

/**
 * 
 * @param float y
 * @param Point p1
 * @param Point p2
 * @return Point 
 */
Point.prototype.yIntersectionLocal = function (y, p1, p2)
{
	this.y = y;
	var dir = p2.subtract(p1);
	var dist = (y - p2.y);
	this.x = p2.x + (dir.x/dir.y) * dist;
	return this;
};

/**
 * @return Point
 */
Point.prototype.normalizeLocal = function()
{
	var l = Math.sqrt(this.x*this.x + this.y*this.y);
	this.x = this.x / l;
	this.y = this.y / l;
	return this;
};

/**
 * @param Point s
 * @return Point
 */
Point.prototype.multiplyLocal = function(s)
{
	this.x = this.x*s;
	this.y = this.y*s;
	return this;
};

/**
 * @param Point p 
 * @return float
 */
Point.prototype.distanceToSquared = function(p)
{
	var dx = this.x-p.x;
	var dy = this.y-p.y;
	return dx*dx + dy*dy;
};


Point.unitTests = function()
{
	module("Point");
	
	test("leftOf tests", function() {
		var point = new Point(10, 10);
		var origo = new Point(0, 0);
		var xaxis = new Point(10, 0);
		var yaxis = new Point(0, 10);
		ok( point.leftOf(origo, xaxis), "Point is left of xaxis");
		ok( !point.leftOf(xaxis, origo), "Point is right of reversed xaxis");
		ok( !point.leftOf(origo, yaxis), "Point is right of yaxis");
		ok( point.leftOf(yaxis, origo), "Point is left of reversed yaxis");
	});

	test("subtract", function() {
		var point = new Point(2, 7.5);
		var p1 = new Point(1, 2);

		equals(point.subtract(p1).x, 1, "X value");
		equals(point.subtract(p1).y, 5.5, "Y value");
	});
	
	test("distanceToSquared", function() {
		var p1 = new Point(1, 1);
		var p2 = new Point(31, 41);

		equals(p1.distanceToSquared(p2), 2500, "Squared-distance");
	});

	test("normalizeLocal", function() {
		var point = new Point(3, 4).normalizeLocal();
		equals(point.x, 3/5, "X value");
		equals(point.y, 4/5, "Y value");

		point = new Point(3, -4).normalizeLocal();
		equals(point.x, 3/5, "X value");
		equals(point.y, -4/5, "Y value");
	});
	
	
	test("xIntersectionLocal", function() {
		var point = new Point(0,0);
		var p1 = new Point(1, 1);
		var p2 = new Point(2, 2);

		point.xIntersectionLocal(0, p1, p2);
		equals(point.y, 0, "Y value");
		point.xIntersectionLocal(0, p2, p1);
		equals(point.y, 0, "Y value");

		p1 = new Point(1, 1);
		p2 = new Point(2, 0);
		point.xIntersectionLocal(0, p1, p2); 
		equals(point.y, 2, "Y value");
		point.xIntersectionLocal(0, p2, p1); 
		equals(point.y, 2, "Y value");
	});

	test("yIntersectionLocal", function() {
		var point = new Point(0,0);
		var p1 = new Point(1, 1);
		var p2 = new Point(2, 2);

		point.yIntersectionLocal(0, p1, p2);
		equals(point.x, 0, "X value");
		point.yIntersectionLocal(0, p2, p1);
		equals(point.x, 0, "X value");

		p1 = new Point(1, 1);
		p2 = new Point(0, 2);
		point.yIntersectionLocal(0, p1, p2); 
		equals(point.x, 2, "X value");
		point.yIntersectionLocal(0, p2, p1); 
		equals(point.x, 2, "X value");
	});
};