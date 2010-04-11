
function LightLayer(light, edges)
{
	this.light = light;
	this.sorter = new EdgeSorter();
	var nearestEdge = sorter.sortByDistance(edges)[0];
	var nearestPoint = nearestEdge.nearestPoint(light);
}