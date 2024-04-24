//
//  LCOnlineLivenessProtocol.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 13.10.2020.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/// Interface of the object which checks Liveness.
@protocol LCOnlineLivenessProtocol <NSObject>


/// Checks the photos for Liveness.
/// @param images List of the best shots.
/// @param completion Result.
- (void)checkOnlineWithImages:(NSArray <UIImage *> *)images completion:(void(^)(int, NSError * _Nullable ))completion;


/// Checks descriptors for track end.
/// @param descriptors List of the best shots descriptors.
/// @param completion Result.
- (void)checkOnlineWithDescriptors:(NSArray <NSData *> *)descriptors completion:(void(^)(BOOL, NSError * _Nullable ))completion;

@end

NS_ASSUME_NONNULL_END
