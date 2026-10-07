// bdc 0x08a9c1e8 g_uiHologramViewPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiHologramViewPhaseTable = {
    { .pfn = (void *)UiHologramViewSetupPhase }, { .pfn = (void *)UiHologramViewWaitPhase },
    { .pfn = (void *)UiHologramViewMainPhase }, { .pfn = (void *)UiHologramViewExitPhase },
};
