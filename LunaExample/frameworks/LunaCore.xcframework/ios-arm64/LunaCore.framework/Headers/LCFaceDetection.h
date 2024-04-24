//
// Created by IVAN CHIRKOV on 14.04.2021.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIImage.h>

NS_ASSUME_NONNULL_BEGIN

/// Object of the face detection.
@interface LCFaceDetection : NSObject

/// Source image.
@property (nonatomic, assign, nonnull) UIImage *sourceImage;
/// Warped image.
@property (nonatomic, assign, nullable) UIImage *warpedImage;
/// Detection rect.
@property (nonatomic, assign) CGRect rect;
/// Score.
@property (nonatomic, assign) CGFloat score;
/// Landmarks.
@property (nonatomic, copy) NSArray *landmarks5;

@end

NS_ASSUME_NONNULL_END
