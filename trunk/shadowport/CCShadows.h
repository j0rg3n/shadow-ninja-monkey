#import "cocos2d.h"


@interface CCShadows : CCLayer 
{
@private
	NSMutableArray* lightNodes;
	NSMutableArray* obstacleNodes;
}

-(id)init;
-(void)dealloc;

-(void)draw;
-(void)addLight:(CCNode*)light;
-(void)addObstacle:(CCNode*)obstacle;

@end
