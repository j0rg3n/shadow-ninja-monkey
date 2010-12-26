#import "CCShadows.h"


#include "FontManager.h";
#include <vector>
#include "MathExtra.h"
#include "Point.h"
#include "Scene.h"
#include "LightCalculator.h"


// ------------------------------------------------------------------------------


static void DrawEdge(const snm::Edge& edge)
{
	ccDrawLine(edge.from, edge.to);
	
	snm::Point normal = snm::Point(-(edge.to.y - edge.from.y), edge.to.x - edge.from.x);
	normal.NormalizeLocal();
	normal.MultiplyLocal(snm::max(edge.Length() / 10, 10));
	
	snm::Point midPoint = snm::Point((edge.from.x + edge.to.x) / 2, (edge.from.y + edge.to.y) / 2);
	
	ccDrawLine(midPoint, midPoint.Add(normal));
}


// ------------------------------------------------------------------------------


@implementation CCShadows

-(id)init
{
	if ((self=[super init])) 
	{
		lightNodes = [[NSMutableArray alloc] init];
		obstacleNodes = [[NSMutableArray alloc] init];
		//[self scheduleUpdate];
	}
	
	return self;
}

-(void)dealloc
{
	[lightNodes release];
	[obstacleNodes release];
	[super dealloc];
}

-(void)draw
{
	/* initial draw state:
	 * glEnableClientState(GL_VERTEX_ARRAY)
	 * glEnableClientState(GL_COLOR_ARRAY)
	 * glEnableClientState(GL_TEXTURE_COORD_ARRAY)
	 * glEnable(GL_TEXTURE_2D)
	 */	 
	CGSize size = [[CCDirector sharedDirector] winSize];
	
	snm::Scene scene;
	scene.AddInverseRectangle(5, 5, size.width - 10, size.height - 10);

	for (CCNode* obstacleNode in obstacleNodes)
	{
		CGRect rect = obstacleNode.boundingBox;
		scene.AddRectangle(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	}
	
	snm::Point light;
	for (CCNode* lightNode in lightNodes)
	{
		light = snm::Point(lightNode.boundingBox.origin.x + lightNode.boundingBox.size.width / 2,
						   lightNode.boundingBox.origin.y + lightNode.boundingBox.size.height / 2);
		
		snm::LightCalculator lightCalculator;
		snm::LightPolygon lightPolygon;
		lightCalculator.CalculateLightPolygon(light, scene, lightPolygon);
		
		const std::vector<snm::Point>& polyPoints = lightPolygon.GetPoints();

		glLineWidth(3);
		for(size_t i = 0; i < polyPoints.size(); ++i)
		{
			size_t j = (i + 1) % polyPoints.size();
			glColor4ub(i == polyPoints.size() - 1 ? 255 : 0,0,255,255);
			DrawEdge(snm::Edge(polyPoints[i], polyPoints[j]));
		}
		glLineWidth(1);
	}
	
	glPointSize(8);
	glColor4ub(0,255,0,255);
	//for(size_t l = 0; l < lights.size(); ++l)
	{
		//const snm::Point& light = lights[l];
		ccDrawPoint(light);
	}
	glPointSize(1);
	
	for(size_t i = 0; i < scene.GetEdges().size(); i++)
	{
		const snm::Edge& edge = scene.GetEdges()[i];
		
		if (light.IsLeftOf(edge.from, edge.to))
		{
			glColor4ub(255,128,128,255);
		}
		else 
		{
			glColor4ub(255,0,0,255);
		}
		
		DrawEdge(edge);
	}
}

-(void)addLight:(CCNode*)light
{
	[lightNodes addObject: light];
}

-(void)addObstacle:(CCNode*)obstacle
{
	[obstacleNodes addObject: obstacle];
}

@end
