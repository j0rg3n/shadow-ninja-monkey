

/**
 * The light calculator calculates a LightLayer.
 * 
 * @return
 */
function LightCalculator(light, scene, edgeSorter)
{
	this.light = light;
	this.scene = scene;
	this.edgeSorter = edgeSorter;
}

LightCalculator.prototype.calculateLight = function()
{
	var visibleEdges = this.scene.visibleEdges(this.light);
	var sortedEdges = this.edgeSorter.sortEdgePoints(light, visibleEdges);
};


LightCalculator.unitTests = function()
{
	module('LightCalculator');

	test("light is passed to scene.visibleEdges", function() {
		var light = new Point(0,0);
		var mockScene = {
				light:null,
				visibleEdges:function(l)
				{
					this.light = l;
				}
		};
		var lightCalculator = new LightCalculator(light, mockScene, new EdgeSorter());
		lightCalculator.calculateLight();
		
		equals( mockScene.light, light, "Something other than the light was passed to visibleEdges");
	});
	test("sortEdgePoints(light,edges) is called with expected params", function() {
		var mockLight = {};
		var mockEdges = {};
		var mockScene = {
				visibleEdges:function(l)
				{
					return mockEdges;
				}
		};
		var mockSorter = {
				light:null,
				edges:null,
				sortEdgePoints:function(l,e)
				{
					this.light = l;
					this.edges = e;
				}
		};
		var lightCalculator = new LightCalculator(mockLight, mockScene, mockSorter);
		lightCalculator.calculateLight();
		
		equals( mockSorter.light, mockLight, "Something other than the light was passed to sortEdgePoints");
		equals( mockSorter.edges, mockEdges, "Something other than the visible edges was passed to sortEdgePoints");
	});
};