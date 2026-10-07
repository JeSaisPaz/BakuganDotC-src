// bdc 0x08a9c698 g_uiGauntletSetupPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiGauntletSetupPhaseTable = {
    { .pfn = (void *)UiGauntletSetupSetupPhase }, { .pfn = (void *)UiGauntletSetupWaitPhase },
    { .pfn = (void *)UiGauntletSetupMainPhase }, { .pfn = (void *)UiGauntletSetupExitPhase },
};
