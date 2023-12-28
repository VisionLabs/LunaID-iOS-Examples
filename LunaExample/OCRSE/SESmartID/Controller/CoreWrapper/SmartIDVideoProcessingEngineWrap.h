//
//  SmartIDRecognitionCoreWrap.h
//  SESmartIDSample
//
//  Created by Artuhay on 22.05.2020.
//  Copyright © 2020 biz.smartengines. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import "SmartIDRecognitionResult.h"
#import "SmartIDSessionSettings.h"
#import "SmartIDVideoProcessingEngine.h"


@interface SmartIDVideoProcessingWrap : UIViewController<smartIDVideoProcessingEngineDelegate>

@property (nonatomic, assign) BOOL canProcessFrames;

@property (nonatomic) float sessionTimeout;

@property (nonatomic, strong) smartIDVideoProcessingEngine * _Nullable videoEngine;

+(nonnull NSString *) version;

- (void) sessionOptionWithOptionName:(nonnull NSString *)optionName andWithOptionValue:(nonnull NSString *)optionValue;

- (void) processImageFile:(nonnull NSString*)filePath;

- (void) processUIImageFile:(nonnull UIImage*)imgFile;

@end
