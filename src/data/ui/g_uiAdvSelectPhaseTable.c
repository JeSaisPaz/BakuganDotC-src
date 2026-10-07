// bdc 0x08a9bdd0 g_uiAdvSelectPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiAdvSelectPhaseTable = {
    { .pfn = (void *)UiAdvSelectSetupPhase }, { .pfn = (void *)UiAdvSelectWaitPhase },
    { .pfn = (void *)UiAdvSelectMainPhase }, { .pfn = (void *)UiAdvSelectExitPhase },
};
