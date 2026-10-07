// bdc 0x08a9c800 g_uiUnlockResultPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiUnlockResultPhaseTable = {
    { .pfn = (void *)UiUnlockResultSetupPhase }, { .pfn = (void *)UiUnlockResultLoadPhase },
    { .pfn = (void *)UiUnlockResultMainPhase }, { .pfn = (void *)UiUnlockResultExitPhase },
};
