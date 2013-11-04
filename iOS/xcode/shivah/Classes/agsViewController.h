
#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <GameKit/GameKit.h>
//#import "GameCenterManager.h"

//@class GameCenterManager;

//@interface Game_CenterViewController : UIViewController <UIActionSheetDelegate, GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GameCenterManagerDelegate> {


@interface agsViewController : UIViewController <UIAlertViewDelegate, UIKeyInput, UITextInputTraits, UIActionSheetDelegate>{
	EAGLContext *context;
	BOOL isInPortraitOrientation;
	BOOL isKeyboardActive;
	BOOL isIPad;
    
    
   // GameCenterManager *gameCenterManager;
    
}

//@property (nonatomic, retain) GameCenterManager *gameCenterManager;

- (void)createGestureRecognizers;
- (void)createKeyboardButtonBar:(int)openedKeylist;
- (void)moveViewAnimated:(BOOL)upwards duration:(float)duration;
- (BOOL) prefersStatusBarHidden;

//- (IBAction) reset;
- (IBAction) showAchievements;
- (void) completeAchievement:(NSString*)identifier value:(double)value;
- (double) isAchieved:(NSString*)identifier;
- (void) resetAchievements;

@end