//
//  LCBestShotDelegate.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 09.10.2020.
//

#import <Foundation/Foundation.h>
#import "LCBestShot.h"

NS_ASSUME_NONNULL_BEGIN

/// Delegate of the best shot detector.
@protocol LCBestShotDelegate <NSObject>

/// Called when a face is detected in the shot.
/// @param rect Detection area.
/// @param frameSize Frame size.
- (void)detectionRect: (CGRect)rect inFrameSize: (CGSize)frameSize;


/// Called when the best shot is found.
/// @note If false is returned, a process of the best shot detection will continue, otherwise the process will be paused. Recall a resume method to continue the process after a pause.
/// @param bestShot The best shot of the face.
/// @return Return true, if the new best shots are no longer needed.
- (BOOL)bestShot:(LCBestShot *)bestShot;


/// Called if an error has occurred.
/// @param error Error.
- (void)bestShotError:(NSError *)error;

@optional


/// Called if check for liveness is active.
/// @param isActive true if check for liveness is currently carried out.
- (void)livenessIsActive:(BOOL)isActive;

@end

NS_ASSUME_NONNULL_END
