//
//  SaveVideoServiceProtocols.h
//  LunaMobile
//
//  Created by michael on 08.08.2023.
//

#ifndef SaveVideoServiceProtocols_h
#define SaveVideoServiceProtocols_h

#import <AVFoundation/AVFoundation.h>
#import "VideoServiceConsts.h"

/// Delegate that should be informed about events inside save video service
@protocol LMSaveVideoServiceDelegate <NSObject>

/// Processing of frames has began. Can be called NOT from main thread
- (void)videoFlushingWillBegin;

/// When all frames are merged into single mp4 video file or in case error aapeared while merging. Can be called NOT from main thread
/// - Parameter error: error object to define the problem
- (void)videoFlushingDidEndWithError: (NSError *) error;

@end

/// This service is intented to save frames one by one and after that flush it into one solid video file
@protocol LMSaveVideoServiceProtocol <NSObject>

/// reset current frames storage
- (void)beginRecordWithFileURL: (NSURL *)fileURL;

/// Append new frame into sequence for future flush into single video file
/// - Parameter sampleBuffer: sample buffer of captured frame
- (void)appendStorageWithSampleBuffer: (CMSampleBufferRef) sampleBuffer;

/// save all the acccumulated frames into video file
/// - Parameter fileURL: resulting video file
- (void)endRecordWithCompletion: (VoidBlock _Nonnull) completion;

@optional
- (void)defineVideoRecordLength: (CGFloat) recordLength;

@end

#endif /* SaveVideoServiceProtocols_h */
