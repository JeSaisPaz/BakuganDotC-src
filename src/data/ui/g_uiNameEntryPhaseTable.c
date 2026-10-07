// bdc 0x08a33914 g_uiNameEntryPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiNameEntryPhaseTable = {
    { .pfn = (void *)UiNameEntryLoadPhase }, { .pfn = (void *)UiNameEntrySetupPhase },
    { .pfn = (void *)UiNameEntryIntroPhase }, { .pfn = (void *)UiNameEntryMainPhase },
    { .pfn = (void *)UiNameEntryConfirmPhase },
};
