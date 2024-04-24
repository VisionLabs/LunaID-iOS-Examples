//
// Created by IVAN CHIRKOV on 25.06.2021.
//

#import <Foundation/Foundation.h>
#import "LCEstimationThreshold.h"
#import "LivenessType.h"

static NSString * const kBestShotLivenessTypeKey = @"kBestShotLivenessTypeKey";
static NSString * const kEyesCheckIDKey = @"kEyesCheckIDKey";

static NSString * const kBorderDistanceKey = @"kBorderDistanceKey";
static NSString * const kMinDetSizeKey = @"kMinDetSizeKey";
static NSString * const kDetectorStepKey = @"kDetectorStepKey";
static NSString * const kSkipFrames = @"kSkipFrames";
static NSString * const kMinimalTrackLength = @"kMinimalTrackLength";
static NSString * const kNumberOfBestshots = @"kNumberOfBestshots";
static NSString * const kBestshotInterval = @"kBestshotInterval";
static NSString * const kLivenessQuality = @"kLivenessQuality";

@interface LCBestShotConfiguration : NSObject

/// Default configuration instance.
+ (instancetype)defaultConfig;

/// Liveness type. This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, assign) LivenessType livenessType;

/// Check eyes condition. If true then best shots with closed eyes will be skipped. This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, assign) BOOL eyesCheck;

/// Distance between face detection and frame edges.
@property (nonatomic, assign) NSInteger borderDistance;

/// Minimum detection size in pixels.
@property (nonatomic, assign) NSInteger minDetSize;

/// The amount of shots between the face detections. The smaller the number is, the more likely that TrackEngine will detect a new face as soon as it appears in the frame. The higher the number is, the higher the overall performance. Used to balance the performance and face detection frequency. Accepted values vary from 0 to 30, 7 by default.
/// The count of frames between frames with full detection, [1 .. 30] ('7' by default).
@property (nonatomic, assign) NSUInteger detectorStep;

/// If a face wasn’t detected in the supposed area, TrackEngine waits this amount of shots before it will finish a track. Values vary from 0 to 50, 36 by default.
@property (nonatomic, assign) NSUInteger skipFrames;

/// Minimal amount of face detections for a track to be considered as a real face. 5 by default.
@property (nonatomic, assign) NSUInteger minimalTrackLength;

/// The amount of the best shots which is necessary to conduct Liveness procedure. 3 by default.
@property (nonatomic, assign) NSUInteger numberOfBestShots;

/// Minimal time interval between the best shots. Value in seconds. 0.5 by default.
/// @see numberOfBestShots
@property (nonatomic, assign) NSTimeInterval bestShotInterval;

/// Liveness quality threshold.
/// If the liveness quality is less than the established value, the liveness is considered not passed. 0 by default.
/// @note Quality value is a probability of "appropriate image". The parameter describes the integral value of image, facial and environmental characteristics.
@property (nonatomic, assign) CGFloat livenessQuality;

/// Configuration object of the best shot estimation.
@property (nonatomic, strong) LCEstimationThreshold *estimationThreshold;

@end
