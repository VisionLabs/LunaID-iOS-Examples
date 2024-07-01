//
//  BestShotConfig.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 13.10.2020.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "LivenessType.h"
#import "LCBestShotConfiguration.h"
#import "LCInteractionsConfig.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LCLunaConfigurationImageType) {
    LCLunaConfigurationImageTypePNG,
    LCLunaConfigurationImageTypeJPEG
};

/// Luna ID SDK configuration.
@interface LCLunaConfiguration : NSObject

/// Returns the default configuration.
+ (instancetype)defaultConfig;

/// Resets license cache.
+ (void)resetLicenseCache;

/// Return configuration from plist file
- (instancetype)initWithPlist:(NSString *)plist;

/// Return configuration from plist file in Documents directory
- (instancetype)initWithPlistFromDocuments:(NSString *)plist;

- (BOOL)hasSettings;
- (NSString *)lunaIDSDKVersion;
- (NSString *)lunaSDKVersion;

/// Makes License activation with some platform specific manner. Network connection is required.
- (NSError * _Nullable)activateLicense;

//  SETTINGS LIST

/// Best shot configuration.
@property (nonatomic, strong) LCBestShotConfiguration *bestShotConfiguration;

@property (nonatomic, strong) LCInteractionsConfig *interactionsConfig;

@property (nonatomic, assign) BOOL glassesCheckEnabled;

/// If the flag is enabled, then a certain number of frames is taken and the median value is checked for the presence of sunglasses
@property (nonatomic, assign) BOOL aggregationEnabled;

/// A flag to include OCR in authorization processes.
@property (nonatomic, assign) BOOL ocrEnabled;

/// Sign of inclusion of interaction in authorization processes. This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, assign) BOOL interactionEnabled;

/// If true then only video with face will be saved. If face is not found in video then such video will not be saved
/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, assign) BOOL saveOnlyFaceVideo;

@property (nonatomic, assign) BOOL trackFaceIdentity;

@property (nonatomic, assign) BOOL occludeCheck;

/// Setting thresholds for sunglasses estimator
@property (nonatomic, assign) BOOL advancedSunglasses;

/// Eye check adjustment. If the flag is set to false, this means that need to make sure that both eyes are open
/// and frames only with all eyes open should be included in the bestshot.
@property (nonatomic, assign) BOOL eyeInjury;

/// This property regulates delay before the face detection starts
@property (nonatomic, assign) CGFloat startDelay;

/// Number of seconds the face should stay in frame before any search of bestshots begin
@property (nonatomic, assign) CGFloat faceTime;

/// Quality of the image sent on the server. Value from 0 to 1. Value 0 is maximum compression, value 1 is no compression. 0.8 by default.
@property (nonatomic, assign) CGFloat compressionQuality;

/// Verification threshold of a document with a best shot. 0.7 by default.
@property (nonatomic, assign) CGFloat documentVerificationMatch;

/// Degree of match between faces, 0.7 by default
@property (nonatomic, assign) CGFloat primaryFaceMatching;

@property (nonatomic, assign) BOOL uploadImagesForLiveness;

/// Sign of displaying the camera switching button. NO by default for Release, YES for Debug.
@property (nonatomic, assign) BOOL isShowSwitchCamera;

/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, strong) NSString *verifyID;

/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, strong) NSString *identifyHandlerID;

/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, strong) NSString *registrationHandlerID;

/// Luna ID authorization token.
/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
/// @note Uses only on APIv4.
@property (nonatomic, strong) NSString *lunaAuthToken;

/// Luna Account ID.
/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
/// @note Uses only on APIv6.
@property (nonatomic, strong) NSString *lunaAccountID;

/// Luna server URL.
/// This property is stored in UserDefaults so any instance of LCLunaConfiguration class will operate with the same value
@property (nonatomic, strong, nullable) NSURL *lunaServerURL;

@property (nonatomic, strong) NSString *plistLicenseFileName;

/// Duration of video in seconds which will be saved, by default 5 seconds. If equal to zero then entire video session will be saved
@property (nonatomic, assign) CGFloat videoRecordLength;

@end

NS_ASSUME_NONNULL_END
