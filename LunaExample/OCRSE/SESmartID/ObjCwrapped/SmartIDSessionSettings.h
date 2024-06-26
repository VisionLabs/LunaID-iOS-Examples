/**
 Copyright (c) 2012-2018, Smart Engines Ltd
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Smart Engines Ltd nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>

@interface SmartIDSessionSettings : NSObject

- (nonnull NSArray<NSString *> *)getEnabledDocumentTypes;

- (void) addEnabledDocumentTypes:(nonnull NSString *)doctypeMask;

- (void) removeEnabledDocumentTypes:(nonnull NSString *)doctypeMask;

- (void)setEnabledDocumentTypes:(nonnull NSArray<NSString *> *)docTypes;

- (nonnull NSArray<NSArray<NSString *> *> *)getSupportedDocumentTypes;

- (nonnull NSDictionary<NSString *, NSString *> *)getOptions;

- (nonnull NSArray<NSString *> *)getOptionNames;

- (BOOL) hasOptionWithName:(nonnull NSString *) name;

- (nonnull NSString *) getOptionWithName:(nonnull NSString *) name;

- (void) setValue:(nonnull NSString *)value toOptionWithName:(nonnull NSString *) name;

- (void) removeOptionWithName:(nonnull NSString *)name;

- (nonnull NSDictionary<NSString *, NSArray<NSString *> *> *)getEnabledFieldNames;

- (void) enableFieldWithType:(nonnull NSString *) doctype andName:(nonnull NSString *) name;

- (void) disableFieldWithType:(nonnull NSString *) doctype andName:(nonnull NSString *) name;

- (void)setEnabledFieldsWithType:(nonnull NSString *)doctype andNames:(nonnull NSArray<NSString *> *)names;

- (nonnull NSArray<NSString *> *)getSupportedFieldNamesForType:(nonnull NSString *)type;

- (nonnull NSDictionary<NSString *, NSArray<NSString *> *> *)getEnabledForensicFieldNames;

- (void) enableForensicFieldWithType:(nonnull NSString *) doctype andName:(nonnull NSString *) name;

- (void) disableForensicFieldWithType:(nonnull NSString *) doctype andName:(nonnull NSString *) name;

- (void)setEnabledForensicFieldsWithType:(nonnull NSString *)doctype andNames:(nonnull NSArray<NSString *> *)names;

- (nonnull NSArray<NSString *> *)getSupportedForensicFieldNamesForType:(nonnull NSString *)type;

- (void) setMode:(nonnull NSString *) mode;

- (nonnull NSArray <NSString *>*) getAvailableModes;

@end

