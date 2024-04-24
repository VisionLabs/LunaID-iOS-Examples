//
//  SmartIDVideoProcessingEngineSwift.h
//  SESmartIDSample
//
//  Created by Artuhay on 22.05.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

#import "SmartIDVideoProcessingEngineWrap.h"

@protocol SmartIDVideoProcessingEngineSwiftDelegate <NSObject>

@optional
- (void) smartIDVideoProcessingEngineDidRecognize:(nonnull SmartIDRecognitionResult *) result fromBuffer:(nonnull CMSampleBufferRef)buffer;

- (void) smartIDVideoProcessingEngineDidRecognize:(nonnull SmartIDRecognitionResult *) result;

- (void) smartIDVideoProcessingEngineDidCancel;

@end

@interface SmartIDVideoProcessingEngineSwift : SmartIDVideoProcessingWrap

@property (weak, nullable) id <SmartIDVideoProcessingEngineSwiftDelegate> smartIDDelegate;

- (nonnull SmartIDSessionSettings *) getSessionSettings;

- (void) addEnabledDocTypesMask:(nonnull NSString *)documentTypesMask;

- (void) removeEnabledDocTypesMask:(nonnull NSString *)documentTypesMask;

- (void) setEnabledDocTypes:(nonnull NSArray<NSString *> *)documentTypes;

- (nonnull NSArray<NSString *> *) enabledDocTypes;

- (nonnull NSArray<NSArray<NSString *> *> *) supportedDocTypes;

- (void) setMode:(nonnull NSString *) stringMode;

- (nonnull NSString *) version;

@end
