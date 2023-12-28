//
//  LCBestShotBuilder.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 07.10.2020.
//

#import <Foundation/Foundation.h>
#import "LCBestShotDetectorProtocol.h"
#import "LCBestShotDelegate.h"
#import "LCLunaConfiguration.h"
#import "LivenessType.h"
#import "LCOnlineLivenessProtocol.h"

NS_ASSUME_NONNULL_BEGIN

/// Detector builder of the best shot.
@interface LCBestShotBuilder : NSObject


/// Creates an instance of the best shot detector.
/// @param delegate Delegate of the best shot detector.
/// @param onlineLiveness Object which checks liveness..
/// @param configuration Luna ID SDK configuration.
+ (id<LCBestShotDetectorProtocol>)buildWithDelegate:(id <LCBestShotDelegate>)delegate
                              livenessAPI:(nullable id <LCOnlineLivenessProtocol>)onlineLiveness
                            configuration:(LCLunaConfiguration *)configuration;

@end

NS_ASSUME_NONNULL_END
