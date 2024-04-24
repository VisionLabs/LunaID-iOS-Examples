//
//  LMCameraCaptureManager.h
//  FaceEngineMobile
//
//  Created on 24.10.16.
//  Copyright © 2016 Vision Labs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "LMCameraCaptureProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@protocol LMCameraCaptureManagerDelegate <NSObject>

- (BOOL)shouldDeleteResultingVideo;
- (void)videoSaveProcessBegan;
- (void)videoSavedWithError: (NSError *) error;

@end

/**
 *  Video stream capture manager.
 */
@interface LMCameraCaptureManager : NSObject<LMCameraCaptureProtocol>

/**
 *  Determines whether orientation change will be processed. By default false.
 *  @note If the property is true, the video stream frame will be expanded  according to the device orientation.
 */
@property (nonatomic) BOOL orientationEnabled;

@property (nonatomic, assign) BOOL recordVideo;
@property (nonatomic, weak) id<LMCameraCaptureManagerDelegate> delegate;

/// Do not touch or change this property, it holds video file name
@property (nonatomic, readonly) NSString *videoFile;

@property (nonatomic, assign) CGFloat fps;
@property (nonatomic, assign) CGFloat frameWidth;
@property (nonatomic, assign) CGFloat frameHeight;

/**
 *  Current camer
 */
@property (nonatomic, readonly) AVCaptureDevicePosition cameraPosition;

/**
 *  Current status of camera a uthorization
 */
+ (AVAuthorizationStatus)cameraAuthorizationStatus;

/**
 *  Requests access to the camera to capture a video stream.
 *  If authorization was passed before, request of authorization will be skipped and current status of authorization will be passed to callback
 *  @note It is required to add to key NSCameraUsageDescription in  Info.plist of project using this framework
 *  @param completion  Callback will be called after authorization passing. Passed parameter with status type of authorization
 */
+ (void)requestAccessForVideoWithCompletion: (void(^)(AVAuthorizationStatus status))completion;

@end
NS_ASSUME_NONNULL_END
