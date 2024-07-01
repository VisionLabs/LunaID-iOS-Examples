//
//  PhotoMaker.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 07.10.2020.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CMSampleBuffer.h>
#import "LCEstimationThreshold.h"
#import "UIKit/UIKit.h"
#import "LCStepConfig.h"

NS_ASSUME_NONNULL_BEGIN

typedef void (^PrimaryFaceFoundHandler)(void);

/// Interface of the best shot detector.
@protocol LCBestShotDetectorProtocol <NSObject>

@required
/// Sends a video stream shot in the processing queue.
/// @note   If a delegate’s method `bestShot` returns `true`, transmitted shots will be ignored unless a `resume` method is called.
/// @param  sampleBuffer Sample of a video stream.
- (void)pushFrameWithSampleBuffer: (CMSampleBufferRef)sampleBuffer;

/// Sends a image in the processing queue. But it works only for one single frame, no video feed or frames sequence support
/// @note   If a delegate’s method `bestShot` returns `true`, transmitted shots will be ignored unless a `resume` method is called.
/// @param  image custom image.
- (void)pushFrameWithImage: (UIImage*)image;

/// Resume the best shot detection process which was suspended through the `bestShot` delegate’s method.x
- (void)resume;

- (void)screenSize:(CGSize) size edges:(UIEdgeInsets) edges;

- (BOOL)faceFound;

- (void) launchWizard;

- (void)defineStepsList: (NSArray<NSObject<LCStepConfigProtocol> *> *) stepsList;

- (void)enableVideoRecordWatchDog: (PrimaryFaceFoundHandler) primaryfaceFoundBlock;

@end

NS_ASSUME_NONNULL_END
