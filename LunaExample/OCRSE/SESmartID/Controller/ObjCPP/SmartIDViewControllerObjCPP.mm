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

#import "SmartIDViewControllerObjCPP.h"
#import "smartIDViewController+Protected.h"
#import "SmartIDRecognitionCore.h"

#import "SmartIDSessionSettings+CPP.h"
#import "SmartIDRecognitionResult+CPP.h"


@interface SmartIDViewControllerCPP () <SmartIDViewControllerProtected>

@end

@implementation SmartIDViewControllerCPP
@synthesize camera, videoEngine, quadrangleView, previewLayer;

- (instancetype)init {
    if (self = [super init]) {
        [[self cancelButton] addTarget:self
                              action:@selector(cancelButtonPressed) forControlEvents:UIControlEventTouchUpInside];
    }
    return self;
}

- (se::smartid::SessionSettings &)sessionSettings {
    return [[[self videoEngine] settings] getUnwrapped];
}

- (void)addEnabledDocumentTypesMask:(const std::string &)documentTypesMask {
    [[[self videoEngine] settings] getUnwrapped].AddEnabledDocumentTypes(documentTypesMask);
}

- (void)removeEnabledDocumentTypesMask:(const std::string &)documentTypesMask {
    [[[self videoEngine] settings] getUnwrapped].RemoveEnabledDocumentTypes(documentTypesMask);
}

- (void)setEnabledDocumentTypes:(const std::vector<std::string> &)documentTypes {
    [[[self videoEngine] settings] getUnwrapped].SetEnabledDocumentTypes(documentTypes);
}

- (const std::vector<std::string> &)enabledDocumentTypes {
    return [[[self videoEngine] settings] getUnwrapped].GetEnabledDocumentTypes();
}

- (const std::vector<std::vector<std::string> > &) supportedDocumentTypes {
    return [[[self videoEngine] settings] getUnwrapped].GetSupportedDocumentTypes();
}

- (void)smartIDVideoProcessingEngineDidRecognizeResult:(SmartIDRecognitionResult *)smartIdResult fromBuffer:(CMSampleBufferRef)buffer {
    [super smartIDVideoProcessingEngineDidRecognizeResult:smartIdResult fromBuffer:buffer];
    if ([[self smartIDDelegate] respondsToSelector:@selector(smartIDViewControllerDidRecognize:fromBuffer:)]) {
        [[self smartIDDelegate] smartIDViewControllerDidRecognize:[smartIdResult getUnwrapped] fromBuffer:buffer];
    }
}

- (void)smartIDVideoProcessingEngineDidRecognizeResult:(SmartIDRecognitionResult *)smartIdResult {
    [super smartIDVideoProcessingEngineDidRecognizeResult:smartIdResult];
    [[self smartIDDelegate] smartIDViewControllerDidRecognize:[smartIdResult getUnwrapped]];
}

- (void)smartIDVideoProcessingEngineDidMatchResult:(NSArray<SmartIDMatchResult *> *)results {
    [super smartIDVideoProcessingEngineDidMatchResult:results];
}

- (void)smartIDVideoProcessingEngineDidSegmentResult:(NSArray<SmartIDSegmentationResult *> *)results {
    [super smartIDVideoProcessingEngineDidSegmentResult:results];
}

- (void)smartIDVideoProcessingEngineDidReceiveFeedback:(SmartIDProcessingFeedback *)processingFeedback {
    [super smartIDVideoProcessingEngineDidReceiveFeedback:processingFeedback];
}

- (void) cancelButtonPressed {
    [[self smartIDDelegate] smartIDviewControllerDidCancel];
}

+ (UIImage *) uiImageFromSmartIdImage:(const se::smartid::Image &)image {
    return [SmartIDRecognitionCore uiImageFromSmartIdImage:image];
}

- (void) setMode:(nonnull NSString *)stringMode {
    [[[self videoEngine] settings] setMode:stringMode];
}

@end
