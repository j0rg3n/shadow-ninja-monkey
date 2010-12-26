//
//  HelloWorldLayer.h
//  shadowport
//
//  Created by Jørgen Havsberg Seland on 09.12.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


// When you import this file, you import all the cocos2d classes
#import "cocos2d.h"

// HelloWorld Layer
@interface HelloWorld : CCLayer
{
@private
	CCSprite* light;
}

// returns a Scene that contains the HelloWorld as the only child
+(id) scene;

@end
