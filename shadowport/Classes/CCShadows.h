#import "cocos2d.h"

@interface CCShadows : CCLayer 
{
@private
	id impl;
}

+(id)shadowsWithDebugScene;
-(id)initWithDebugScene;
-(void)draw;
-(void)update:(ccTime)deltaTime;

@end
