//
//  LMCameraCaptureProtocol.h
//  LunaMobile
//
//  Created by michael on 05.05.2023.
//

#ifndef LMCameraCaptureProtocol_h
#define LMCameraCaptureProtocol_h

#import "VideoServiceConsts.h"

typedef void (^CaptureSampleBufferHandler)(CMSampleBufferRef _Nonnull );
typedef void (^CaptureLayerHandler)(AVCaptureVideoPreviewLayer * _Nullable);

@protocol LMCameraCaptureProtocol <NSObject>

/**
 *  Called video capture from telephone camera Вызывает захват видеопотока с камеры телефона.
 *  @note Use the previewLayer property to display the video stream on the UI. Use the captureHandler property to capture the frame buffer of the video stream.
 *  @see previewLayer
 *  @see captureHandler
 */
- (void)start;

/**
 *  Stops capturing a video stream.
 */
- (void)stopWithCompletion: (VoidBlock _Nonnull) completion;

- (void)updateLayerFrameWithRect: (CGRect) rect;

/**
 *  Configures a video stream capture session.
 *  @note If the device does not have a camera in the selected position or does not support the 1280x720 preset, the method returns false.
 *  @param cameraPosition Camera position
 */
- (void)configureWithCameraPosition: (AVCaptureDevicePosition)cameraPosition
                         completion: (VoidBlock _Nonnull) completion;

- (void)findPreviewLayerWithCompletion: (CaptureLayerHandler _Nonnull) completion;

/**
 *  Changes the camera type between main and front.
 */
- (void)switchCameraType;

- (void)defineSampleBufferhandler: (CaptureSampleBufferHandler)newHandler;

@end

#endif /* LMCameraCaptureProtocol_h */
