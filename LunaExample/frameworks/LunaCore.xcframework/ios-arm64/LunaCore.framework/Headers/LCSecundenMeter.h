//
//  LCSecundenMeter.h
//  LunaCore
//
//  Created by michael on 05.12.2023.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

@interface LCSecundenMeter : NSObject

- (instancetype)initWithDelay: (CGFloat) delay;
- (instancetype)initWithSecunden: (LCSecundenMeter *) oldSecunden;
- (void)launch;
- (void)launchWithDelay: (NSTimeInterval) delay;
- (BOOL)alreadyLaunched;
- (BOOL)expired;
- (NSTimeInterval)expireTime;

@end

NS_ASSUME_NONNULL_END
