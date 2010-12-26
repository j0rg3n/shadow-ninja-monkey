#import "HelloWorldScene.h"
#import "CCShadows.h"
#import "CCTouchDispatcher.h"

@implementation HelloWorld

+(id) scene
{
	// 'scene' is an autorelease object.
	CCScene *scene = [CCScene node];
	
	// 'layer' is an autorelease object.
	HelloWorld *layer = [HelloWorld node];
	
	// add layer as a child to scene
	[scene addChild: layer];
	
	// return the scene
	return scene;
}

-(id) init
{
	if( (self=[super init] )) {
		
		CCShadows* shadowLayer = [[CCShadows alloc] init];
		[self addChild:shadowLayer];
		
		CCSprite* light = [CCSprite spriteWithFile: @"Icon.png"];
		light.position = ccp(50, 50);
		light.scale = .25f;
		[shadowLayer addLight:light];
		[self addChild:light];

		CCSprite* obstacle = [CCSprite spriteWithFile: @"Icon.png"];
		obstacle.position = ccp(150, 50);
		[shadowLayer addObstacle:obstacle];
		[self addChild:obstacle];

		obstacle = [CCSprite spriteWithFile: @"Icon.png"];
		obstacle.position = ccp(250, 50);
		obstacle.scale = .45f;
		[shadowLayer addObstacle:obstacle];
		[self addChild:obstacle];
					
		obstacle = [CCSprite spriteWithFile: @"Icon.png"];
		obstacle.position = ccp(250, 250);
		obstacle.scale = 1.45f;
		[shadowLayer addObstacle:obstacle];
		[self addChild:obstacle];

		self.isTouchEnabled = TRUE;
	}
	return self;
}

-(void) registerWithTouchDispatcher
{
	[[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:0 swallowsTouches:YES];
}

- (BOOL)ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event {
	
	CCNode* node;
	CCARRAY_FOREACH (self.children, node)
	{
		if ([node class] != [CCSprite class])
		{
			continue;
		}
		
		CGPoint location = [self convertTouchToNodeSpace: touch];
		CGRect rect = node.boundingBox;
		
		if (location.x >= rect.origin.x && 
			location.y >= rect.origin.y &&
			location.x < rect.origin.x + rect.size.width &&
			location.y < rect.origin.y + rect.size.height)
		{
			touchedNode = node;
			return YES;
		}
	}
	
    return NO;
}

- (void)ccTouchMoved:(UITouch *)touch withEvent:(UIEvent *)event {
	CGPoint location = [self convertTouchToNodeSpace: touch];
	touchedNode.position = location;
}

- (void)ccTouchEnded:(UITouch *)touch withEvent:(UIEvent *)event {
	CGPoint location = [self convertTouchToNodeSpace: touch];
	touchedNode.position = location;
	touchedNode = nil;
}

// on "dealloc" you need to release all your retained objects
- (void) dealloc
{
	// in case you have something to dealloc, do it in this method
	// in this particular example nothing needs to be released.
	// cocos2d will automatically release all the children (Label)
	
	// don't forget to call "super dealloc"
	[super dealloc];
}
@end
