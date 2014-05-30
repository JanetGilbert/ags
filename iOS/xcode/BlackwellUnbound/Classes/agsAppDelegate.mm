
#import "agsAppDelegate.h"
#import "agsViewController.h"

/*#define TESTFLIGHT 1

#ifdef TESTFLIGHT
#import "TestFlight.h"
#endif*/


//#import <Crashlytics/Crashlytics.h>


@implementation agsAppDelegate

@synthesize window;
@synthesize viewController;

volatile int is_in_foreground = 1;
volatile int drawing_in_progress = 0;

extern "C" void ios_resume_sound();

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{

    
	[[UIApplication sharedApplication] setIdleTimerDisabled:YES];
	self.window.rootViewController = self.viewController;
    
/*#ifdef TESTFLIGHT
    [TestFlight setDeviceIdentifier:[[UIDevice currentDevice] uniqueIdentifier]]; //j must NOT be in release build
    [TestFlight takeOff:@"8897920d3d667b72496a97c996cc98c0_MTYyMzU0MjAxMi0xMi0wNCAxNjo1MDozOS4wMzM4MjY"];
#endif*/
  //  [Crashlytics startWithAPIKey:@"632ccb316e88010c5d7b793d4bf069e57fbf624e"];
	return YES; 
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	is_in_foreground = 0;
	while (drawing_in_progress)
	{
		//printf("waiting for drawing to finish...\n");
		usleep(1000 * 100);
	}
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    is_in_foreground = 1;
    ios_resume_sound();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
}


- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Handle any background procedures not related to animation here.
	is_in_foreground = 0;
    glFinish();
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
