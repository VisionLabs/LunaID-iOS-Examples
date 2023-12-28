//
//  SmartIDVideoProcessingEngineSwift.m
//  SESmartIDSample
//
//  Created by Artuhay on 22.05.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

#import "SmartIDVideoProcessingEngineSwift.h"
#import "SmartIDSessionSettings+CPP.h"

@implementation SmartIDVideoProcessingEngineSwift

- (instancetype) init {
  self = [super init];
  return self;
}

- (SmartIDSessionSettings *) getSessionSettings {
    return [[self videoEngine] settings];
}

- (void) addEnabledDocTypesMask:(NSString *)documentTypesMask {
    [[[self videoEngine] settings] addEnabledDocumentTypes:documentTypesMask];
}
- (void) removeEnabledDocTypesMask:(NSString *)documentTypesMask {
    [[[self videoEngine] settings] removeEnabledDocumentTypes:documentTypesMask];
}

- (void) setEnabledDocTypes:(NSArray<NSString *> *)documentTypes {
    [[[self videoEngine] settings] setEnabledDocumentTypes:documentTypes];
}
- (NSArray<NSString *> *) enabledDocTypes {
    return [[[self videoEngine] settings] getEnabledDocumentTypes];
}

- (NSArray<NSArray<NSString *> *> *) supportedDocTypes {
    return [[[self videoEngine] settings] getSupportedDocumentTypes];
}

- (void)smartIDVideoProcessingEngineDidRecognizeResult:(SmartIDRecognitionResult *)smartIdResult fromBuffer:(CMSampleBufferRef)buffer {
    [super smartIDVideoProcessingEngineDidRecognizeResult:smartIdResult fromBuffer:buffer];
    if ([[self smartIDDelegate] respondsToSelector:@selector(smartIDVideoProcessingEngineDidRecognize:fromBuffer:)]) {
        [[self smartIDDelegate] smartIDVideoProcessingEngineDidRecognize:smartIdResult fromBuffer:buffer];
    }
}

- (void)smartIDVideoProcessingEngineDidRecognizeResult:(SmartIDRecognitionResult *)smartIdResult {
    [super smartIDVideoProcessingEngineDidRecognizeResult:smartIdResult];
    [[self smartIDDelegate] smartIDVideoProcessingEngineDidRecognize:smartIdResult];
}

- (void)smartIDVideoProcessingEngineDidSegmentResult:(NSArray<SmartIDSegmentationResult *> *)quads {
    //[super smartIDVideoProcessingEngineDidSegmentResult:quads];
}

- (void)smartIDVideoProcessingEngineDidReceiveFeedback:(SmartIDProcessingFeedback *)processingFeedback {
    //[super smartIDVideoProcessingEngineDidReceiveFeedback:processingFeedback];
}

- (void)smartIDVideoProcessingEngineDidMatchResult:(NSArray<SmartIDMatchResult *> *)quads {
    //[super smartIDVideoProcessingEngineDidMatchResult:quads];
}

- (void) cancelButtonPressed {
    [[self smartIDDelegate] smartIDVideoProcessingEngineDidCancel];
}

- (void) setMode:(nonnull NSString *)stringMode {
    [[[self videoEngine] settings] setMode:stringMode];
}


- (nonnull NSString *)version {
	return [smartIDVideoProcessingEngine version];
}

@end

