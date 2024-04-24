//
//  LCDescriptorExtractorBuilder.h
//  LunaCore
//
//  Created by IVAN CHIRKOV on 12.10.2020.
//

#import <Foundation/Foundation.h>
#import "LCDescriptorExtractorProtocol.h"
#import "LCLunaConfiguration.h"

NS_ASSUME_NONNULL_BEGIN

/// Builder of descriptors extractor.
@interface LCDescriptorExtractorBuilder : NSObject

/// Creates an instance of descriptor’s extractor.
+ (id<LCDescriptorExtractorProtocol>)buildWithConfiguration: (LCLunaConfiguration *) configuration;

@end

NS_ASSUME_NONNULL_END
