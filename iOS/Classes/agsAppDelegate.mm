#import "agsAppDelegate.h"
#import "agsViewController.h"


@implementation agsAppDelegate

@synthesize window;
@synthesize viewController;



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    /*[[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    self.window.rootViewController = self.viewController;
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    */
    return YES;
}

- (void)dealloc
{
	[viewController release];
	[window release];
	
	[super dealloc];
}

#pragma mark - UISceneSession Lifecycle

- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
    // Returns the configuration matching your Info.plist setup
    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
}


@end
