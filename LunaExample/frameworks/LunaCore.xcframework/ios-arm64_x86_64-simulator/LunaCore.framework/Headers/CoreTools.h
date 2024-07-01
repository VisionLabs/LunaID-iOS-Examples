//
//  CoreTools.h
//  LunaCore
//
//  Created by michael on 21.02.2024.
//

#ifndef CoreTools_h
#define CoreTools_h

#include <stdarg.h>
#import <Foundation/Foundation.h>

#if LUNA_LOG

typedef NS_ENUM(NSUInteger, ELogMessageType) {
    LOG_MESSAGE_TYPE_WARNING,
    LOG_MESSAGE_TYPE_INFO,
    LOG_MESSAGE_TYPE_ERROR
};

@interface LCThreadSafeLog : NSObject

+ (instancetype)shared;

- (void)logInfoMessageWithFormat: (NSString *) format, ...;

@end

#define DLog(...) NSLog(__VA_ARGS__)
#define DLogInfo(...) [[LCThreadSafeLog shared] logInfoMessageWithFormat: __VA_ARGS__]; NSLog(__VA_ARGS__)
#else
#define DLog(...)
#define DLogInfo(...)
#endif

#endif /* CoreTools_h */
