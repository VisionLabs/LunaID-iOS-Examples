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

/// Interface of the best shot detector.
@protocol LCBestShotDetectorProtocol <NSObject>

@required
/// Sends a video stream shot in the processing queue.
/// @note   If a delegate’s method `bestShot` returns `true`, transmitted shots will be ignored unless a `resume` method is called.
/// @param  sampleBuffer Sample of a video stream.
- (void)pushFrame: (CMSampleBufferRef)sampleBuffer;

/// Resume the best shot detection process which was suspended through the `bestShot` delegate’s method.x
- (void)resume;


- (void)screenSize:(CGSize) size edges:(UIEdgeInsets) edges;

- (BOOL)faceFound;

- (void) launchWizard;

- (void)defineStepsList: (NSArray<NSObject<LCStepConfigProtocol> *> *) stepsList;


@end

NS_ASSUME_NONNULL_END
