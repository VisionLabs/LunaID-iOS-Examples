//
// Created by IVAN CHIRKOV on 14.04.2021.
//

#import <Foundation/Foundation.h>

@class UIImage;
@class LCFaceDetection;

NS_ASSUME_NONNULL_BEGIN

@protocol LCFaceDetectorProtocol <NSObject>

/// Detects faces in the image
/// @param image Source image.
/// @param maxCount Maximum number of faces to be detected.
- (NSArray<LCFaceDetection *> *)detectFaces: (UIImage *)image maxCount: (NSUInteger) maxCount;

@end

NS_ASSUME_NONNULL_END
