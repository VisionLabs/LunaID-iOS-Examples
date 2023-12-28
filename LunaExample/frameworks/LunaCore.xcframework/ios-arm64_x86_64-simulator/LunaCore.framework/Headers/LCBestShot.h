//
//  BestShot.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 08.10.2020.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/// Object of the best shot.
@interface LCBestShot : NSObject


/// Returns the best shot image.
/// @param hasWarped If true, the image will be normalized (warped).
- (UIImage *)getUIImageWarped:(BOOL)hasWarped;


/// Returns a face detection area relative to the shot..
- (CGRect)getDetection;


/// Returns a shot’s AGS. AGS in the range from 0 to 1.
- (float)getAGS;

- (void)processUIImage;

@end

NS_ASSUME_NONNULL_END
