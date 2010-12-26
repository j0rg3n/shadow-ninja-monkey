#import "cocos2d.h"


@interface CCShadows : CCLayer 
{
@private
	id sceneWrapper;
	NSMutableArray* lightNodes;
	float time;
}

+(id)shadowsWithDebugScene;
-(id)initWithDebugScene;
-(void)dealloc;

-(void)draw;
-(void)update:(ccTime)deltaTime;
-(void)addLight:(CCNode*)light;

@end
