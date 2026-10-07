// bdc 0x08a9fab4 g_uiComboListPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiComboListPhaseTable = {
    { .pfn = (void *)UiComboListLoadPhase }, { .pfn = (void *)UiComboListOpenPhase },
    { .pfn = (void *)UiComboListMainPhase }, { .pfn = (void *)UiComboListExitPhase },
};
