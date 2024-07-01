//
//  LCEstimationThreshold.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 09.10.2020.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

/// Configuration object of the best shot estimation.
@interface LCEstimationThreshold : NSObject


/// Limits the head rotation along the X axis.
@property (nonatomic) CGFloat headPitch;


/// Limits the head rotation along the Y axis.
@property (nonatomic) CGFloat headYaw;

/// Limits the head rotation along the Z axis.
@property (nonatomic) CGFloat headRoll;


/// Minimum value of shot’s quality estimation.
@property (nonatomic) CGFloat ags;


+ (instancetype)defaultConfig;

/// Initialization.
/// @param headPitch Limits the head rotation along the X axis.
/// @param headYaw Limits the head rotation along the Y axis.
/// @param headRoll Limits the head rotation along the Z axis.
/// @param ags Minimum value of shot’s quality estimation.
- (instancetype)initWithHeadPitch: (CGFloat)headPitch
                          headYaw: (CGFloat)headYaw
                         headRoll: (CGFloat)headRoll
                              ags: (CGFloat)ags;

@end

NS_ASSUME_NONNULL_END
