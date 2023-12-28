//
//  Errors.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 09.10.2020.
//
#import <Foundation/Foundation.h>
#import "NSString+Localizable.h"

#ifndef LUNACORE_BESTSHOTERRORS_H
#define LUNACORE_BESTSHOTERRORS_H

/// List of error codes of the LunaCore module.
typedef NS_ENUM(NSInteger, BestShotError) {
    /// LunaCore module initialization error.
    INITIALIZATION_ERROR = 1000,
    /// Bad quality.
    BAD_QUALITY = 1001,
    /// The user’s head is turned too much.
    BAD_HEAD_POSE = 1002,
    /// Multiple faces were detected in the frame.
    MULTIPLE_FACES = 1003,
    /// Liveness check has not been passed.
    LIVENESS_ERROR = 1004,
    /// A face has not been found.
    TRACK_END = 1005,
    /// Need to blink.
    NEED_TO_BLINK = 1006,
    /// Interaction timeout.
    INTERACTION_TIMEOUT = 1007,
    /// Medical mask is on the face.
    MASK = 1008,
    /// Mask is not on the right place.
    MASK_NOT_IN_PLACE = 1009,
    /// Face is occluded by something.
    OCCLUDED_FACE = 1010,
    /// The image is blurred.
    IMAGE_IS_BLURRED = 1011,
    /// The image is underexposed (i.e., too dark).
    IMAGE_IS_UNDEREXPOSED = 1012,
    /// The image is overexposed(i.e., too light).
    IMAGE_IS_OVEREXPOSED = 1013,
    /// The face in the image is illuminated unevenly (there is a great difference between light and dark regions).
    IMAGE_IS_ILLUMINATED_UNEVENLY = 1014,
    /// Image contains flares on face (too specular).
    IMAGE_IS_TOO_SPECULAR = 1015,
    /// The face is too far.
    TOO_FAR = 1016,
    /// The face ovelapses borders.
    BORDERS = 1017,
    ROTATE_HEAD_LEFT = 1018,
    ROTATE_HEAD_RIGHT = 1019,
    ROTATE_HEAD_DOWN = 1020,
    ROTATE_HEAD_UP = 1021,
    FACE_LOST = 1022,
    PRIMARY_FACE_LOST = 1023,
    SUNGLASSES_DETECTED = 1024
};

/// Error domain of the LunaCore module.
static NSString * const kBestShotErrorDomain = @"ru.visionlabs.lunacore.error.domain";

#endif
