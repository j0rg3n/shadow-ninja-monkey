#import "CCShadows.h"


#include <vector>
#include "Point.h"


@interface CCShadowsImpl : NSObject
{
@private
	std::vector<snm::Point> lights;
}

+(id)shadowsImplWithDebugScene;
-(id)initWithDebugScene;
-(void)dealloc;
-(void)draw;
-(void)update;
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
		
		/*
		 var scene = new Scene(800, 600);
		 scene.addLightBoundingBox();
		 scene.addRectangle(400, 200, 100, 25);
		 scene.addRectangle(40, 150, 40, 35);
		 scene.addRectangle(250, 300, 200, 25);
		 scene.addRectangle(50, 500, 200, 50);
		 
		 for(var i = 0; i < 5; i++)
		 {
		 scene.addRectangle(150+Math.sin(i), 50+i*40, 40+i*25, 10);
		 }
		 */
	}
	return self;
}

-(void)dealloc
{
	lights.clear();
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
	
	
	// draw a simple line
	// The default state is:
	// Line Width: 1
	// color: 255,255,255,255 (white, non-transparent)
	// Anti-Aliased
	glEnable(GL_LINE_SMOOTH);
	ccDrawLine( ccp(0, 0), ccp(s.width, s.height) );
	glDisable(GL_LINE_SMOOTH);
	
	/*
	 // line: color, width, aliased
	 // glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
	 // GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
	 glDisable(GL_LINE_SMOOTH);
	 glLineWidth( 5.0f );
	 glColor4ub(255,0,0,255);
	 ccDrawLine( ccp(0, s.height), ccp(s.width, 0) );
	 
	 // TIP:
	 // If you are going to use always the same color or width, you don't
	 // need to call it before every draw
	 //
	 // Remember: OpenGL is a state-machine.
	 
	 // draw big point in the center
	 glPointSize(64);
	 glColor4ub(0,0,255,128);
	 ccDrawPoint( ccp(s.width / 2, s.height / 2) );
	 
	 // draw 4 small points
	 CGPoint points[] = { ccp(60,60), ccp(70,70), ccp(60,70), ccp(70,60) };
	 glPointSize(4);
	 glColor4ub(0,255,255,255);
	 ccDrawPoints( points, 4);
	 
	 // draw a green circle with 10 segments
	 glLineWidth(16);
	 glColor4ub(0, 255, 0, 255);
	 ccDrawCircle( ccp(s.width/2,  s.height/2), 100, 0, 10, NO);
	 
	 // draw a green circle with 50 segments with line to center
	 glLineWidth(2);
	 glColor4ub(0, 255, 255, 255);
	 ccDrawCircle( ccp(s.width/2, s.height/2), 50, CC_DEGREES_TO_RADIANS(90), 50, YES);	
	 
	 // open yellow poly
	 glColor4ub(255, 255, 0, 255);
	 glLineWidth(10);
	 CGPoint vertices[] = { ccp(0,0), ccp(50,50), ccp(100,50), ccp(100,100), ccp(50,100) };
	 ccDrawPoly( vertices, 5, NO);
	 
	 // closed purble poly
	 glColor4ub(255, 0, 255, 255);
	 glLineWidth(2);
	 CGPoint vertices2[] = { ccp(30,130), ccp(30,230), ccp(50,200) };
	 ccDrawPoly( vertices2, 3, YES);
	 
	 // draw quad bezier path
	 ccDrawQuadBezier(ccp(0,s.height), ccp(s.width/2,s.height/2), ccp(s.width,s.height), 50);
	 
	 // draw cubic bezier path
	 ccDrawCubicBezier(ccp(s.width/2, s.height/2), ccp(s.width/2+30,s.height/2+50), ccp(s.width/2+60,s.height/2-50),ccp(s.width, s.height/2),100);
	 
	 
	 // restore original values
	 glLineWidth(1);
	 glColor4ub(255,255,255,255);
	 glPointSize(1);
	 */
}

-(void)update
{
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
	[i update];
}

@end
