//
//  SmartIDRecognitionCoreWrap.m
//  SESmartIDSample
//
//  Created by Artuhay on 22.05.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

#import "SmartIDVideoProcessingEngineWrap.h"

@implementation SmartIDVideoProcessingWrap

- (void) commonInitialize {
    self.videoEngine = [[smartIDVideoProcessingEngine alloc] init];
}

+ (NSString *) version {
    return [smartIDVideoProcessingEngine version];
}

- (instancetype) init {
    if (self = [super init]) {
        [self commonInitialize];
    }
    return self;
}


- (void)smartIDVideoProcessingEngineDidProcessSnapshot {}

- (void)smartIDVideoProcessingEngineDidRejectSnapshot {}

- (void)smartIDVideoProcessingEngineDidCancel {}

- (void) smartIDVideoProcessingEngineDidRecognizeResult:(nonnull SmartIDRecognitionResult *)smartIdResult fromBuffer:(nonnull CMSampleBufferRef)buffer{}

- (void) smartIDVideoProcessingEngineDidRecognizeResult:(nonnull SmartIDRecognitionResult *)smartIdResult{
  if (![self canProcessFrames]) {
      return;
  }
  if ([smartIdResult isTerminal]) {
      [self setCanProcessFrames:NO];
  }
}

-(void) sessionOptionWithOptionName:(nonnull NSString *)optionName andWithOptionValue:(nonnull NSString *)optionValue{
   [[[self videoEngine] settings] setValue:optionValue toOptionWithName:optionName];
}

- (void) processImageFile:(nonnull NSString*)filePath{
  __weak typeof(self) weaksef = self;
  [[self videoEngine] setDelegate:weaksef];
  [[self videoEngine] processImageFilePath:filePath];
}

- (void) processUIImageFile:(nonnull UIImage*)imgFile{
  __weak typeof(self) weakself = self;
  [[self videoEngine] setDelegate:weakself];
  SmartIDImage *smImage = [[SmartIDImage alloc] initWithImage:imgFile];
  [[self videoEngine] processImage:smImage];
}

@end
