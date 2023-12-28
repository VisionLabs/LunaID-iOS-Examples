//
//  LivenessType.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 13.10.2020.
//

#import <Foundation/Foundation.h>

/// Liveness type.
typedef NS_ENUM(NSUInteger, LivenessType) {
    /// Liveness disabled.
    LivenessType_None,
    /// Liveness which uses photos.
    LivenessType_ByPhoto,
    /// Liveness which uses descriptors.
    /// @note This is not online liveness!
    LivenessType_Offline
};
