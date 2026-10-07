// bdc 0x08a9b560 g_uiRepairPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiRepairPhaseTable = {
    { .pfn = (void *)UiRepairWaitPhase }, { .pfn = (void *)UiRepairSetupPhase },
    { .pfn = (void *)UiRepairMainPhase }, { .pfn = (void *)UiRepairExitPhase },
};
