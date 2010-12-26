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


/**
 * Wrapper around C++ state to avoid exposing this in the header.
 */
@interface SceneWrapper : NSObject
{	
}

+(id)sceneWrapperWithDebugScene;
-(id)initWithDebugScene;
-(void)dealloc;

@property (readonly, nonatomic) snm::Scene* scene;
@end

@implementation SceneWrapper

@synthesize scene;

+(id)sceneWrapperWithDebugScene
{
	return [[self alloc] initWithDebugScene];
}

-(id)initWithDebugScene
{
	if ((self=[super init])) 
	{
		scene = new snm::Scene(800, 600);
		scene->AddLightBoundingBox();
		scene->AddRectangle(400, 200, 100, 25);
		//scene->AddRectangle(40, 150, 40, 35);
		//scene->AddRectangle(250, 300, 200, 25);
		//scene->AddRectangle(50, 500, 200, 50);
		
		//for(int i = 0; i < 5; i++)
		{
			//scene->AddRectangle(150+sin(i), 50+i*40, 40+i*25, 10);
		}
	}
	return self;
}

-(void)dealloc
{
	delete scene;
	[super dealloc];
}

@end

@implementation CCShadows

+(id)shadowsWithDebugScene
{
	return [[self alloc] initWithDebugScene];
}

-(id)initWithDebugScene
{
	if ((self=[super init])) 
	{
		sceneWrapper = [SceneWrapper sceneWrapperWithDebugScene];
		lightNodes = [[NSMutableArray alloc] init];
		[self scheduleUpdate];
	}
	
	return self;
}

-(void)dealloc
{
	[lightNodes release];
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
	CGSize s = [[CCDirector sharedDirector] winSize];
	
	const snm::Scene& scene = *((SceneWrapper*) sceneWrapper).scene;
	
	NSEnumerator* e = [lightNodes objectEnumerator];
	
	snm::Point light;
	while (true)
	{
		CCNode* lightNode = [e nextObject];
		if (!lightNode)
		{
			break;
		}
		
		light = [self convertToNodeSpace:[lightNode convertToWorldSpace:lightNode.position]];
		
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

-(void)update:(ccTime)deltaTime
{
}

-(void)addLight:(CCNode*)light
{
	[lightNodes addObject: light];
}

@end
