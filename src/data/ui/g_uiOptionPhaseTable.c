// bdc 0x08a9d9f8 g_uiOptionPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiOptionPhaseTable = {
    { .pfn = (void *)UiOptionPhaseStart }, { .pfn = (void *)UiOptionPhaseLoad },
    { .pfn = (void *)UiOptionMainPhase }, { .pfn = (void *)UiOptionPhaseClose },
    { .pfn = (void *)UiOptionNetMainPhase },
};
