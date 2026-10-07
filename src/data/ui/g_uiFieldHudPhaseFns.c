// bdc 0x08a94ebc g_uiFieldHudPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiFieldHudPhaseFns = {
    { .pfn = (void *)UiFieldHudInitPhase }, { .pfn = (void *)UiFieldHudSetupPhase },
    { .pfn = (void *)UiFieldHudMainPhase }, { .pfn = (void *)UiFieldHudStage32Phase },
    { .pfn = (void *)UiFieldHudStopPhase },
};
