
#import "agsAppDelegate.h"
#import "agsViewController.h"

/*#define TESTFLIGHT 1

#ifdef TESTFLIGHT
#import "TestFlight.h"
#endif*/


#import <Crashlytics/Crashlytics.h>


@implementation agsAppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

    
	[[UIApplication sharedApplication] setIdleTimerDisabled:YES];
	self.window.rootViewController = self.viewController;
    
/*#ifdef TESTFLIGHT
    [TestFlight setDeviceIdentifier:[[UIDevice currentDevice] uniqueIdentifier]]; //j must NOT be in release build
    [TestFlight takeOff:@"8897920d3d667b72496a97c996cc98c0_MTYyMzU0MjAxMi0xMi0wNCAxNjo1MDozOS4wMzM4MjY"];
#endif*/
    [Crashlytics startWithAPIKey:@"632ccb316e88010c5d7b793d4bf069e57fbf624e"];
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)applicationWillTerminate:(UIApplication *)application
{
}

int is_in_foreground = 1;

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Handle any background procedures not related to animation here.
	is_in_foreground = 0;
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Handle any foreground procedures not related to animation here.
	is_in_foreground = 1;
}

- (void)dealloc
{
	[viewController release];
	[window release];
	
	[super dealloc];
}

@end
