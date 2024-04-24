//
//  LCStepConfig.h
//  LunaCore
//
//  Created by michael on 31.10.2023.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

static NSTimeInterval const DefaultInteractionTimeout = 5;
static NSTimeInterval const DefaultTimeoutBetweenInteractions = 0;

NS_ASSUME_NONNULL_BEGIN

@protocol LCStepConfigProtocol <NSObject>

@property (nonatomic, assign) CGFloat timeout;

@end

@interface LCBlinkConfig : NSObject<LCStepConfigProtocol>

@property (nonatomic, assign) CGFloat timeout;

@end

@interface LCUpHeadTrackConfig : NSObject<LCStepConfigProtocol>

@property (nonatomic, assign) CGFloat timeout;
@property (nonatomic, assign) CGFloat startAngle;
@property (nonatomic, assign) CGFloat finishAngle;

@end

@interface LCDownHeadTrackConfig : NSObject<LCStepConfigProtocol>

@property (nonatomic, assign) CGFloat timeout;
@property (nonatomic, assign) CGFloat startAngle;
@property (nonatomic, assign) CGFloat finishAngle;

@end

@interface LCLeftHeadTrackConfig : NSObject<LCStepConfigProtocol>

@property (nonatomic, assign) CGFloat timeout;
@property (nonatomic, assign) CGFloat startAngle;
@property (nonatomic, assign) CGFloat finishAngle;

@end

@interface LCRightHeadTrackConfig : NSObject<LCStepConfigProtocol>

@property (nonatomic, assign) CGFloat timeout;
@property (nonatomic, assign) CGFloat startAngle;
@property (nonatomic, assign) CGFloat finishAngle;

@end

NS_ASSUME_NONNULL_END
