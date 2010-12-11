#ifndef _LIGHTPOLYGON_H_INCLUDED
#define _LIGHTPOLYGON_H_INCLUDED


#include <vector>
#include "Point.h"


namespace snm
{
	
	
class LightPolygon
{
public:
	/**
	 * Add a point to the light-polygon.
	 * 
	 * @param Point p
	 * @return
	 */
	void AddPoint(const Point& p)
	{
		if(this->polygon.size() == 0 || this->polygon[this->polygon.size()-1].DistanceToSquared(p) > 0.00001)
		{
			this->polygon.push_back(p);
		}
	};
	
	/**
	 * Get the polygon
	 * 
	 * @return Point[] - The array of points that describe the light polygon.
	 */
	const std::vector<Point>& GetPoints() const
	{
		return this->polygon;
	};
	
private:
	std::vector<Point> polygon;
	
	/*
	 LightPolygon.unitTests()
	 {
	 module('LightPolygon');
	 
	 test("addPoint and getPoints", function() {
	 var lightPolygon = new LightPolygon();
	 lightPolygon.addPoint(new Point(42, 43));
	 lightPolygon.addPoint(new Point(14, -56));
	 var poly = lightPolygon.getPoints();
	 equals( poly.length, 2, "Two points in the polygon");
	 equals( poly[0].x, 42, "x-value of point[0]");
	 equals( poly[0].y, 43, "y-value of point[0]");
	 equals( poly[1].x, 14, "x-value of point[1]");
	 equals( poly[1].y, -56, "y-value of point[1]");
	 });
	 
	 test("addPoint eliminates points that are the same", function() {
	 var lightPolygon = new LightPolygon();
	 lightPolygon.addPoint(new Point(42, 43));
	 lightPolygon.addPoint(new Point(42, 43));
	 var poly = lightPolygon.getPoints();
	 equals( poly.length, 1, "One point in the polygon");
	 equals( poly[0].x, 42, "x-value of point[0]");
	 equals( poly[0].y, 43, "y-value of point[0]");
	 });
	 
	 };
	 */
};

}; // namespace snm	

#endif // _LIGHTPOLYGON_H_INCLUDED