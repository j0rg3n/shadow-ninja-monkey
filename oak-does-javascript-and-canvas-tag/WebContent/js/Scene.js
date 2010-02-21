
function Scene(width,height)
{
	this.width = width;
	this.height = height;
	this.edges = [];
	this.lights = [];
}

Scene.prototype.addLightBoundingBox = function()
{
	// Add an inverse box around the scene to avoid light getting 'out'
	this.addEdge(new Point(0,   0+h), new Point(0,   0));
	this.addEdge(new Point(0+w, 0+h), new Point(0,   0+h));
	this.addEdge(new Point(0+w, 0),   new Point(0+w, 0+h));
	this.addEdge(new Point(0,   0),   new Point(0+w, 0));
};

Scene.prototype.addEdge = function(p1, p2)
{
	this.edges.push(new Edge(p1, p2));
};

Scene.prototype.addRectangle = function(x, y, w, h)
{
	this.addEdge(new Point(x,   y),   new Point(x,   y+h));
	this.addEdge(new Point(x,   y+h), new Point(x+w, y+h));
	this.addEdge(new Point(x+w, y+h), new Point(x+w, y));
	this.addEdge(new Point(x+w, y),   new Point(x,   y));
};

Scene.prototype.visibleEdges = function(light)
{
	var result = [];
	for(var i = 0; i < this.edges.length; i++)
	{
		var e = this.edges[i];
		if(e instanceof Edge && e.isVisible(light))
		{
			result.push(e);
		}
	}
	return result;
};

Scene.unitTests = function()
{
	module('Scene');
	
	test("addEdge", function() {
		var scene = new Scene(100, 100);
		var from = new Point(9, 9);
		var to = new Point(11, 9);
		
		scene.addEdge(from, to);
		equals( scene.edges.length, 1, "Edge count");
		equals( scene.edges[0].from, from, "From point");
		equals( scene.edges[0].to, to, "To point");
	});
	
	test("visibleEdges (visible)", function(){
		var scene = new Scene(100, 100);
		scene.addEdge(new Point(1, 1), new Point(3, 1));
		
		var edges = scene.visibleEdges(new Point(2, 2));
		equals(edges.length, 1, 'Visible edge accepted');
	});
	test("visibleEdges (visible)", function(){
		var scene = new Scene(100, 100);
		scene.addEdge(new Point(1, 3), new Point(3, 3));
		
		var edges = scene.visibleEdges(new Point(2, 2));
		equals(edges.length, 0, 'Invisible edge declined');
	});
};
