// bdc 0x08a9ed68 g_uiUnlockCodePhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[6]) g_uiUnlockCodePhaseFns = {
    { .pfn = (void *)UiUnlockCodeLoadPhase }, { .pfn = (void *)UiUnlockCodeSetupPhase },
    { .pfn = (void *)UiUnlockCodeIntroPhase }, { .pfn = (void *)UiUnlockCodeInputPhase },
    { .pfn = (void *)UiUnlockCodeCheckPhase }, { .pfn = (void *)UiUnlockCodeExitPhase },
};
