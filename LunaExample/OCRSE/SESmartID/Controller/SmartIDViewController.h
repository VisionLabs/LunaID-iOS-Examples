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


#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import "SmartIDRecognitionResult.h"
#import "SmartIDSessionSettings.h"
#import "SmartIDVideoProcessingEngine.h"
#import "SmartIDCaptureButton.h"

@class DocTypeLabel;
@class ImagePickerFromSmartIDViewController;

@interface SmartIDViewController : UIViewController<smartIDVideoProcessingEngineDelegate, SmartIDCameraButtonDelegate>

@property (nonatomic, assign) BOOL displayDocumentQuadrangle;
@property (nonatomic, assign) BOOL displayZonesQuadrangles;
@property (nonatomic, assign) BOOL displayProcessingFeedback;
@property (nonatomic, assign) BOOL shouldDisplayRoi;
@property (nonatomic, assign) float quadranglesAlpha;
@property (nonatomic, assign) float quadranglesWidth;
@property (nonatomic, strong, nonnull) UIColor * quadranglesColor;
@property (nonatomic, strong, nonnull) SmartIDCaptureButton *captureButton;
@property (nonatomic, weak, nullable) id<SmartIDCameraButtonDelegate> captureButtonDelegate;

@property (nonatomic) float sessionTimeout;

@property (nonatomic, nonnull) UIButton *cancelButton;

// optinal elements
@property (nonatomic, strong, nullable) DocTypeLabel *docTypeLabel;
@property (nonatomic, strong, nullable) UIButton *galleryButton;
@property (nonatomic, strong, nullable) ImagePickerFromSmartIDViewController *photoLibImagePicker;

-(void)setDefaultOrientation:(UIDeviceOrientation)orientation; // if orientation lock enabled

-(void)startRecognition;

-(void)stopRecognition;

+(nonnull NSString *) version;

-(CGSize)cameraSize;

-(void)setRoiWithOffsetX:(CGFloat)offsetX andY:(CGFloat)offsetY orientation:(UIDeviceOrientation)orientation;

-(void) sessionOptionWithOptionName:(nonnull NSString *)optionName andWithOptionValue:(nonnull NSString *)optionValue;

- (void) processImageFile:(nonnull NSString*)filePath;

- (void) processUIImageFile:(nonnull UIImage*)imgFile;

@end



