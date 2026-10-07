// bdc 0x08a9ced0 g_uiScreen390PhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiScreen390PhaseFns = {
    { .pfn = (void *)UiScreen390InitPhase }, { .pfn = (void *)UiScreen390FadeInPhase },
    { .pfn = (void *)UiScreen390MainPhase }, { .pfn = (void *)UiScreen390ExitPhase },
};
