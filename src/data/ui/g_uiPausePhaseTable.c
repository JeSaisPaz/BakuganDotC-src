// bdc 0x08a9b5b8 g_uiPausePhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[7]) g_uiPausePhaseTable = {
    { .pfn = (void *)UiPauseWaitPhase }, { .pfn = (void *)UiPauseSetupPhase },
    { .pfn = (void *)UiPauseMainPhase }, { .pfn = (void *)UiPauseExitPhase },
    { .pfn = (void *)UiPauseOpenComboListPhase }, { .pfn = (void *)UiPauseReturnPhase },
    { .pfn = (void *)UiPauseWaitComboListPhase },
};
