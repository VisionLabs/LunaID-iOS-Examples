//
// Created by IVAN CHIRKOV on 14.04.2021.
//

#import <Foundation/Foundation.h>

#import "LCFaceDetectorProtocol.h"
#import "LCLunaConfiguration.h"

NS_ASSUME_NONNULL_BEGIN

@interface LCFaceDetectorBuilder : NSObject

+ (id<LCFaceDetectorProtocol>)buildWithConfiguration: (LCLunaConfiguration *) configuration;

@end

NS_ASSUME_NONNULL_END
