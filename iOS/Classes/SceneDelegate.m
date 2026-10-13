//
//  SceneDelegate.m
//  BlackwellUnbound
//
//  Created by Janet Gilbert on 10/13/26.
//
extern void ios_resume_sound(void);
extern volatile int ios_audio_must_restart;

#import "SceneDelegate.h"
#import "agsViewController.h"
#import "agsAppDelegate.h"

@interface NibOwner : NSObject
@property (strong) id delegate;
@end
@implementation NibOwner
@end

@implementation SceneDelegate

volatile int is_in_foreground = 1;
volatile int drawing_in_progress = 0;


- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)opts {
    NibOwner *o = [NibOwner new];
    NSString *nib = (UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPad) ? @"MainWindow-iPad" : @"MainWindow";
    [[NSBundle mainBundle] loadNibNamed:nib owner:o options:nil];

    agsAppDelegate *ad = (agsAppDelegate *)UIApplication.sharedApplication.delegate;
    ad.window = [o.delegate window];
    self.window = ad.window;
    self.window.windowScene = (UIWindowScene *)scene;
    [self.window makeKeyAndVisible];
}


- (void)sceneDidBecomeActive:(UIScene *)scene {
    // Migrated from applicationDidBecomeActive:
    is_in_foreground = 1;
    ios_audio_must_restart = 1;
    usleep(1000 * 1000); //jg Crude hack to try and make the audio start up after answering a phone call.
    ios_resume_sound();
}

- (void)sceneWillResignActive:(UIScene *)scene {
    do
    {
        //printf("waiting for drawing to finish %d...\n", drawing_in_progress);
        usleep(1000 * 100);
    }
    while (drawing_in_progress);
}

- (void)sceneWillEnterForeground:(UIScene *)scene {
    // Handle any foreground procedures not related to animation here.
    is_in_foreground = 1;
}

- (void)sceneDidEnterBackground:(UIScene *)scene {
    // Handle any background procedures not related to animation here.
    is_in_foreground = 0;
    while (drawing_in_progress)
    {
        //printf("waiting for drawing to finish (background) %d...\n", drawing_in_progress);
        usleep(1000 * 100);
    };
    glFinish();
}

@end
