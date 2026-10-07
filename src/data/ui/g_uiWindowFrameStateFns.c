// bdc 0x08ac6200 g_uiWindowFrameStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_uiWindowFrameStateFns = {
    { .pfn = (void *)UiWindowFrameStepIdle }, { .pfn = (void *)UiWindowFrameStepOpen },
    { .pfn = (void *)UiWindowFrameStepClose },
};
