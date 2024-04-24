//
//  LCInteractionsConfig.h
//  LunaCore
//
//  Created by michael on 26.09.2023.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

static NSString * _Nonnull const kTimeoutBetweenInteractionsKey = @"kTimeoutBetweenInteractionKey";
static NSString * const kInteractionTimeoutKey = @"kInteractionTimeoutKey";
static NSString * const kStepsNumberKey = @"kStepsNumberKey";

@interface LCInteractionsConfig : NSObject

/// Default configuration instance.
+ (instancetype)defaultConfig;

/// This property defines timeout for every interaction.
@property (nonatomic, assign) CGFloat interactionTimeout;

/// number of steps which should be generated in random order
@property (nonatomic, assign) NSInteger stepsNumber;

@property (nonatomic, assign) CGFloat timeoutBetweenInteractions;

@end

NS_ASSUME_NONNULL_END
