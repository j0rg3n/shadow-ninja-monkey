#import "cocos2d.h"


@interface CCShadows : CCLayer 
{
@private
	NSMutableArray* lightNodes;
	NSMutableArray* obstacleNodes;
}

@property (readwrite) CGRect shadowBounds;


-(id)init;
-(void)dealloc;

-(void)draw;
-(void)addLight:(CCNode*)light;
-(void)addObstacle:(CCNode*)obstacle;

@end
