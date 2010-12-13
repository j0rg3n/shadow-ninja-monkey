#import "CCShadows.h"


#include <vector>
#include "Point.h"
#include "Scene.h"
#include "LightCalculator.h"


@interface CCShadowsImpl : NSObject
{
@private
	std::vector<snm::Point> lights;
	snm::Scene* scene;
	float time;
}

+(id)shadowsImplWithDebugScene;
-(id)initWithDebugScene;
-(void)dealloc;
-(void)draw;
-(void)update:(ccTime)deltaTime;
@end

@implementation CCShadowsImpl

+(id)shadowsImplWithDebugScene
{
	return [[self alloc] initWithDebugScene];
}

-(id)initWithDebugScene
{
	if ((self=[super init])) 
	{
		// setup the scene
		lights.push_back(snm::Point(35, 75));
		lights.push_back(snm::Point(75, 35));
		lights.push_back(snm::Point(75, 35));
		
		scene = new snm::Scene(800, 600);
		scene->AddLightBoundingBox();
		scene->AddRectangle(400, 200, 100, 25);
		scene->AddRectangle(40, 150, 40, 35);
		scene->AddRectangle(250, 300, 200, 25);
		scene->AddRectangle(50, 500, 200, 50);
		
		for(int i = 0; i < 5; i++)
		{
			scene->AddRectangle(150+sin(i), 50+i*40, 40+i*25, 10);
		}
	}
	return self;
}

-(void)dealloc
{
	lights.clear();
	delete scene;
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

	for(size_t l = 0; l < lights.size(); ++l)
	{
		const snm::Point& light = lights[l];
		snm::LightCalculator lightCalculator;
		snm::LightPolygon lightPolygon;
		lightCalculator.CalculateLightPolygon(light, *scene, lightPolygon);
		
		const std::vector<snm::Point> &polyPoints = lightPolygon.GetPoints();
		for(size_t i = 0; i < polyPoints.size(); ++i)
		{
			size_t j = (i + 1) % polyPoints.size();
			ccDrawLine(polyPoints[i], polyPoints[j]);
		}
	}
	
	glPointSize(8);
	glColor4ub(0,0,255,128);
	for(size_t l = 0; l < lights.size(); ++l)
	{
		const snm::Point& light = lights[l];
		ccDrawPoint(light);
	}
	glPointSize(1);
	
	for(size_t i = 0; i < scene->GetEdges().size(); i++)
	{
		const snm::Edge& edge = scene->GetEdges()[i];
		ccDrawLine(edge.from, edge.to);
	}
}

-(void)update:(ccTime)deltaTime
{
	CGSize s = [[CCDirector sharedDirector] winSize];

	time += deltaTime;
	time = fmod(time, M_PI * 200);
	
	float w = s.width;
	float h = s.height;
	lights[0].x = w / 2 + sin(time) * w / 3;
	lights[0].y = h / 2 + cos(time) * h / 3;
	lights[1].x = w / 2 + sin(time * 1.01 + M_PI * 1.95) * w / 2.5;
	lights[1].y = h / 2 + cos(time * 1.01 + M_PI * 1.95) * h / 2.5;
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
		impl = [CCShadowsImpl shadowsImplWithDebugScene];
		[self scheduleUpdate];
	}
	
	return self;
}

-(void)draw
{
	CCShadowsImpl* i = impl;
	[i draw];
}

-(void)update:(ccTime)deltaTime
{
	CCShadowsImpl* i = impl;
	[i update:deltaTime];
}

@end
