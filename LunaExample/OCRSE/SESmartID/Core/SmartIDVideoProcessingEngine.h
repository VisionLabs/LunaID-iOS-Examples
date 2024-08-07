/**
 Copyright (c) 2012-2018, Smart Engines Ltd
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Smart Engines Ltd nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "SmartIDRecognitionResult.h"
#import "SmartIDSessionSettings.h"
#import "SmartIDQuadrangle.h"
#import "SmartIDMatchResult.h"
#import "SmartIDSegmentationResult.h"
#import "SmartIDProcessingFeedback.h"

@protocol smartIDVideoProcessingEngineDelegate <NSObject>

@optional
- (void) smartIDVideoProcessingEngineDidRecognizeResult:(nonnull SmartIDRecognitionResult *)smartIdResult
                                             fromBuffer:(nonnull CMSampleBufferRef)buffer;

- (void) smartIDVideoProcessingEngineDidRecognizeResult:(nonnull SmartIDRecognitionResult *)smartIdResult;

- (void) smartIDVideoProcessingEngineDidMatchResult:(nonnull NSArray<SmartIDMatchResult *> *)results;

- (void) smartIDVideoProcessingEngineDidSegmentResult:(nonnull NSArray<SmartIDSegmentationResult *> *)results;

- (void) smartIDVideoProcessingEngineDidReceiveFeedback:(nonnull SmartIDProcessingFeedback *)processingFeedback;

- (void) smartIDVideoProcessingEngineDidProcessSnapshot;

- (void) smartIDVideoProcessingEngineDidRejectSnapshot;

- (void) smartIDVideoProcessingEngineDidCancel;

@end

@interface smartIDVideoProcessingEngine : NSObject

@property (weak, nullable) id<smartIDVideoProcessingEngineDelegate> delegate; // receives recognition callbacks

+ (nonnull NSString *) version;

- (void) startSession:(CGSize)cameraPreset;

- (BOOL) isSessionRunning;

- (void) endSession;

- (void) processSampleBuffer:(nonnull CMSampleBufferRef)sampleBuffer withOrientation:(UIDeviceOrientation)deviceOrientation;

- (void) processSampleBuffer:(nonnull CMSampleBufferRef)sampleBuffer withOrientation:(UIDeviceOrientation)deviceOrientation andRoi:(CGRect)roi;

- (void) processImageFilePath:(nonnull NSString*)filePath;

- (void) processImage:(nonnull SmartIDImage*)imageFile;

- (void) processUIImage:(nonnull UIImage*)image;


- (nonnull SmartIDSessionSettings *) settings;

@end

