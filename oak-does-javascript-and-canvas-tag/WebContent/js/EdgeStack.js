

/**
 * A stack of edges sorted by their distance to the light-source.
 * 
 * @param Point light
 * @return
 */
function EdgeStack(light)
{
	this.light = light;
	this.edges = [];
}

EdgeStack.prototype.addEdgePoint = function(p)
{
	if(p == p.edge.from)
	{
		for(var i = 0; i < this.edges.length; i++)
		{
			if(p.edge.distanceToPointSquared(this.light) < this.edges[i].distanceToPointSquared(this.light))
			{
				this.edges.splice(i, 0, p.edge);
				return;
			}
		}
		this.edges.push(p.edge);
	}
	else
	{
		// Remove the edge
		for(var i = 0; i < this.edges.length; i++)
		{
			if(this.edges[i] == p.edge)
			{
				this.edges.splice(i, 1);
				break;
			}
		}
	}
};

/**
 * This will initiate the stack such that it contains all the edges that the light would pass
 * through if a line be drawn between the light-source and the first point of the edge start/end points.
 * 
 * @param light
 * @param sortedEdges
 * @return
 */
EdgeStack.prototype.initStack = function(sortedEdges)
{
	this.edges = [];
	for(var i = 0; i < sortedEdges.length; i++)
	{
		this.addEdgePoint(sortedEdges[i]);
	}
};


EdgeStack.unitTests = function()
{
	module('EdgeStack');

	test("addEdgePoint add one edge", function() {
		var stack = new EdgeStack(new Point(0,0));
		var e1 = new Edge(new Point(2, 1), new Point(0, 1));
		stack.addEdgePoint(e1.from);
		
		equals( stack.edges.length, 1, "There should be one edge");
		equals( stack.edges[0], e1, "It should be e1");
	});
	
	test("addEdgePoint add two edges", function() {
		var stack = new EdgeStack(new Point(0,0));
		var e1 = new Edge(new Point(2, 1), new Point(0, 1));
		var e2 = new Edge(new Point(2, 2), new Point(0, 2));
		stack.addEdgePoint(e1.from);
		stack.addEdgePoint(e2.from);
		
		equals( stack.edges.length, 2, "There should be two edges");
		equals( stack.edges[0], e1, "It should be e1");
		equals( stack.edges[1], e2, "It should be e1");
	});
	
	test("addEdgePoint remove e2", function() {
		var stack = new EdgeStack(new Point(0,0));
		var e1 = new Edge(new Point(2, 1), new Point(0, 1));
		var e2 = new Edge(new Point(2, 2), new Point(0, 2));
		stack.addEdgePoint(e1.from);
		stack.addEdgePoint(e2.from);
		stack.addEdgePoint(e2.to);
		
		equals( stack.edges.length, 1, "There should be one edge");
		equals( stack.edges[0], e1, "It should be e1");
	});
	
	test("addEdgePoint remove e1", function() {
		var stack = new EdgeStack(new Point(0,0));
		var e1 = new Edge(new Point(2, 1), new Point(0, 1));
		var e2 = new Edge(new Point(2, 2), new Point(0, 2));
		stack.addEdgePoint(e1.from);
		stack.addEdgePoint(e2.from);
		stack.addEdgePoint(e1.to);
		
		equals( stack.edges.length, 1, "There should be one edge");
		equals( stack.edges[0], e2, "It should be e2");
	});
	
	test("initStack 1 edge example (from -> to)", function() {
		var stack = new EdgeStack(new Point(0,0));
		var e1 = new Edge(new Point(2, 1), new Point(0, 1));
		var sortedEdges = [e1.from, e1.to];
		stack.initStack(sortedEdges);
		
		equals( stack.edges.length, 0, "There should be no edges in the stack");
	});
	
	test("initStack 1 edge example (to -> from)", function() {
		var stack = new EdgeStack(new Point(0,0));
		var e1 = new Edge(new Point(2, 1), new Point(0, 1));
		var sortedEdges = [e1.to, e1.from];
		stack.initStack(sortedEdges);
		
		equals( stack.edges.length, 1, "There should be one edge in the stack");
		equals( stack.edges[0], e1, "The edge is e1");
	});
	
	test("initStack 3 edge example", function() {
		var e0 = new Edge(new Point(1, 3), new Point(-1, 3));
		var e1 = new Edge(new Point(1, 2), new Point(-1, 2));
		var e2 = new Edge(new Point(1, 1), new Point(-1, 1));
		var light = new Point(0,0);
		var sortedEdges = [e0.from, e1.from, e1.to, e2.from];
		var stack = new EdgeStack(new Point(0,0));
		stack.initStack(sortedEdges);
		
		equals( stack.edges.length, 2, "There should be two edges");
		equals( stack.edges[0], e2, "It should be e2");
		equals( stack.edges[1], e0, "It should be e0");
	});	
};