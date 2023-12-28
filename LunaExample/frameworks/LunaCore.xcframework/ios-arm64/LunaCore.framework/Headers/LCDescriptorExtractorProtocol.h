//
//  LCDescriptorExtractorProtocol.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 12.10.2020.
//

#import <Foundation/Foundation.h>
#import "LCBestShot.h"

@class LCFaceDetection;

NS_ASSUME_NONNULL_BEGIN

/// Interface of descriptor extractor.
@protocol LCDescriptorExtractorProtocol <NSObject>

/// Extracts descriptor from the best shot.
/// @param bestShot Object of the best shot.
- (NSData *)extractDescriptorDataFromBestShot:(LCBestShot *)bestShot;

/// Extracts descriptor from the face detection.
/// @param faceDetection Object of the face detection.
- (NSData *)extractDescriptorDataFromFaceDetection:(LCFaceDetection *)faceDetection;

/// Compares best shot and face detection.
/// @return Returns comparison value from 0 to 1. The higher value is, the more similar descriptors are.
- (float)match:(LCBestShot *)bestShot and:(LCFaceDetection *)faceDetection;

/// Compares two descriptors.
/// @return Returns comparison value from 0 to 1. The higher value is, the more similar descriptors are.
- (float)matchDescriptorsData:(NSData *)firstDescriptorData and:(NSData *)secondDescriptorData __unused;

@end

NS_ASSUME_NONNULL_END
