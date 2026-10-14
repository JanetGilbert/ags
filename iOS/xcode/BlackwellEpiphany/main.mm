
#import <UIKit/UIKit.h>
#import <stdio.h>
#import "agsAppDelegate.h"


int main(int argc, char *argv[])
{
	// Disable buffering for stdout
	setvbuf(stdout, NULL, _IONBF, 0);

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	int result = UIApplicationMain(argc, argv, nil, NSStringFromClass([agsAppDelegate class]));
	[pool release];

	return result;
}
