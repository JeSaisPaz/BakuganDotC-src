// bdc 0x08a9b500 g_uiConfirmDialogPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiConfirmDialogPhaseTable = {
    { .pfn = (void *)UiConfirmDialogWaitPhase }, { .pfn = (void *)UiConfirmDialogSetupPhase },
    { .pfn = (void *)UiConfirmDialogMainPhase }, { .pfn = (void *)UiConfirmDialogExitPhase },
};
