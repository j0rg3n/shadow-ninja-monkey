//
//  main.m
//  shadowport
//
//  Created by Jørgen Havsberg Seland on 09.12.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	int retVal = UIApplicationMain(argc, argv, nil, @"shadowportAppDelegate");
	[pool release];
	return retVal;
}
