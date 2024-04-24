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
#define DLog(...) NSLog(__VA_ARGS__)
#else
#define DLog(...)
#endif

#endif /* CoreTools_h */
