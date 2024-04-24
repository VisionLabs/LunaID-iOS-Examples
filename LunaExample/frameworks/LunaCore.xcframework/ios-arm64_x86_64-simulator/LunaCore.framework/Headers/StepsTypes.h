//
//  StepsTypes.h
//  LunaCore
//
//  Created by michael on 25.12.2023.
//

#ifndef StepsTypes_h
#define StepsTypes_h


typedef NS_ENUM(NSUInteger, LCWizardStatus) {
    LCWizardIdle,
    LCWizardProcessing,
    LCWizardFinished
};

typedef NS_ENUM(NSUInteger, LCStepState) {
    LCStepStarted,
    LCStepInProgress,
    LCStepFinished
};

extern NSErrorUserInfoKey const NSStepStateKey;

#endif /* StepsTypes_h */
